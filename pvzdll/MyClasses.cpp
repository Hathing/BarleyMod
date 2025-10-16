#include "MyClasses.hpp"

void PVZ::ApplyPZDamage(MyPlant plant, MyZombie zombie, int damage, PVZ::DamageFlags flags)
{
	PZDamageEvent event{ zombie, plant, flags, damage, PVZEvent::PLANTDAMAGETYPE_CUSTOM};
	onPlantDamageZombie(&event);
	event.zombie.Hit(event.damage, event.flags);
}


void PVZ::ApplyZPDamage(MyZombie zombie, MyPlant plant, int damage)
{
	int mydmg = onPlantTakeDamage(plant, zombie, GameObjectType::OBJECT_TYPE_NONE, damage);
	plant.Hp -= mydmg;
	//这里可以增加死亡判断，自定义死亡类型，否则植物会在原版总更新中因HP < 0 而死。
}