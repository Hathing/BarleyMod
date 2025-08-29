#include "pch.h"
#include "MyPlant/PlantAbility.hpp"
#include "MyZombie/ZombieAbility.hpp"
#include "MyEvents.hpp"

void onZombieDropLoot(MyZombie zombie)
{
	zombie.DroppedLoot = 1;

	if (zombie.Type == ZombieType::DrZomboss)
		return;
	
	if (zombie.PoisonStack >= 10)
	{
		int spread_stack = zombie.PoisonStack >> 2;
		auto zombies = zombie.GetBoard().GetAllZombies<MyZombie>();
		for (auto myzombie : zombies)
			if (myzombie.Row == zombie.Row && myzombie.X - 80 >= zombie.X && myzombie.X + 80 <= zombie.X
					&& myzombie.Type != ZombieType::DrZomboss)
				myzombie.PoisonStack += spread_stack;
	}

	if (!zombie.Hypnotized)
	{
		int bounty_xp = zombie.GetBountyXP();
		auto attacker = MyPlant::GetByID(zombie.LastDamageSourceID).GetOwner();

		if (attacker.isValid() && attacker.IsXPRecipient())
		{
			PlantAbility::GetPrototype(attacker.Type)->onKill(attacker, zombie);
			attacker.AddExperience(bounty_xp * 4 / 5, true);
			bounty_xp -= (bounty_xp * 4 / 5);
		}

		auto plants = zombie.GetBoard().GetAllPlants<MyPlant>();
		int plant_cnt = 0;
		for (auto plant : plants)
			if (plant.Row == zombie.Row && plant.IsXPRecipient())
				plant_cnt++;
		if (plant_cnt <= 0) return;
		bounty_xp /= plant_cnt;
		for (auto plant : plants)
			if (plant.Row == zombie.Row && plant.IsXPRecipient())
				plant.AddExperience(bounty_xp);
	}
}

void onZombieInitAfter(MyZombie zombie)
{
	zombie.LastDamageSourceID = 0;
	ZombieAbility::GetAbility(zombie.Type)->onCreated(zombie);
}

void onRandomZombieDropHelm(MyZombie zombie)
{
	if (zombie.Type != ZombieType::ConeheadZombie) {
		zombie.HelmType = HelmType::None;
		return;
	}
	zombie.Remove();
	int type = Creator::Rand(33);
	if (type == 25)type = 26;//僵王换豌豆
	if (type == 20)type = 27;//蹦极换坚果
	auto child_zombie = Creator::CreateZombie(static_cast<ZombieType::ZombieType>(type), zombie.Row, 0x0f);
	PVZ::CreateParticleSystem(zombie.X + 40.0f,zombie.Y + 65.0f,zombie.Layer+100,EffectType::IMITATER_TRANSFORMING);
	child_zombie.X = zombie.X;
	float health_ratio = (1 + Creator::Rand(5)) / 5.0f;
	child_zombie.BodyHealth *= health_ratio;
	child_zombie.BodyMaxHealth *= health_ratio;
	child_zombie.HelmHealth *= health_ratio;
	child_zombie.HelmMaxHealth *= health_ratio;
	child_zombie.ShieldHealth *= health_ratio;
	child_zombie.ShieldMaxHealth *= health_ratio;
	///651185跳到了651373和6512d5,不知道是干啥的一段代码，没搬
	return;
}

int onPlantTakeDamage(MyPlant plant, PVZ::BaseClass source, GameObjectType::GameObjectType source_type, int damage)
{
	return damage;
}

bool onZombieSquishPlant(MyZombie zombie, int row, int column, int attack_type, MyPlant plant)
{
	auto zombietype = zombie.Type;
	auto planttype = plant.Type;
	if (zombietype == ZombieType::Zomboin || zombietype == ZombieType::CatapultZombie)
	{
		if (planttype == SeedType::Wallnut || planttype == SeedType::Tallnut)
		{
			zombie.X += 50.0f;//击退距离
			//这里应该将 僵尸伤害植物 和 植物伤害僵尸 和 子弹伤害僵尸 分别封装成一个函数，避免某些原本该触发的事件未触发
			zombie.Hit(200, PVZ::DAMAGEF_NONE);
			int damage = onPlantTakeDamage(plant, zombie, GameObjectType::OBJECT_TYPE_NONE, 500);//这里object type没有僵尸？
			plant.Hp -= damage;//植物碾压受伤
			return false;
		}
	}
	return true;
}

void onLoadPlainZombieReanimBefore(MyZombie zombie)
{
	//隐藏鬼火
	zombie.ReanimShowPrefix("guih", -1);
	zombie.ReanimShowPrefix("wenhao", -1);
}

float onZombieUpdateWalkingSpeed(MyZombie zombie, float velocity)
{
	if (zombie.X > zombie.GetBoard().GetIcetrace().GetX(zombie.Row) - 40)
		return velocity * 2.0f;
	return velocity;
}

void InitZombieEvents()
{
	PlantTakeDamageEvent((int)onPlantTakeDamage);
	ZombieDropLootEvent((int)onZombieDropLoot);
	ZombieInitAfterEvent((int)onZombieInitAfter);
	PVZEvent::RandomZombieDropHelmEvent((int)onRandomZombieDropHelm);
	PVZEvent::ZombieSquishPlantEvent((int)onZombieSquishPlant);
	PVZEvent::LoadPlainZombieReanimBeforeEvent((int)onLoadPlainZombieReanimBefore);
	PVZEvent::ZombieUpdateWalkingSpeedEvent((int)onZombieUpdateWalkingSpeed);
	//修改冰道持续时间
	PVZ::Memory::WriteMemoryUnsafe<int>(0x52A8B6, 1000);
}