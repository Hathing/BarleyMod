#include "pch.h"
#include "PlantEvents.cpp"
#include "ZombieEvents.cpp"
#include "ProjectileEvents.cpp"

//这个文件仅仅用于定义那些需要调用事件触发的MyClass的成员函数！

int MyPlant::DoDamageToZombie(MyZombie zombie,PVZ::DamageFlags flags, int damage, PVZEvent::PlantDamageType damage_type)
{
	//这里可以有修改伤害的事件，目前没有
	if (damage > 0)
	{
		//这里也可以有一个修改僵尸受到伤害的事件
		zombie.Hit(damage, flags);
		PZDamageEvent event{ zombie, *this, flags, damage,damage_type };
		onPlantDamageZombie(&event);
		return damage;
	}
	return 0;
}

int MyPlant::TakeDamage(PVZ::BaseClass source, GameObjectType::GameObjectType source_type, int damage)
{
	int result_damage = onPlantTakeDamage(*this,  source, source_type, damage);
	if (result_damage > 0)
	{
		plant.Hp -= result_damage;
		return result_damage;
	}
	return 0;
}

//还有僵尸对植物造成伤害、僵尸受到伤害、子弹对僵尸造成伤害......