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
		bounty_xp /= plant_cnt;
		for (auto plant : plants)
			if (plant.Row == zombie.Row && plant.IsXPRecipient())
				plant.AddExperience(bounty_xp);
	}
}

void onZombieInitAfter(MyZombie zombie)
{
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
	///6510FA这里需要创建一团雾气
	child_zombie.X = zombie.X;
	float health_ratio = (float)Creator::Rand(33) / 5.0f;
	child_zombie.BodyHealth *= health_ratio;
	child_zombie.BodyMaxHealth *= health_ratio;
	child_zombie.HelmHealth *= health_ratio;
	child_zombie.HelmMaxHealth *= health_ratio;
	child_zombie.ShieldHealth *= health_ratio;
	child_zombie.ShieldMaxHealth *= health_ratio;
	///651185跳到了651373和6512d5,不知道是干啥的一段代码，没搬
	return;
}

void InitZombieEvents()
{
	ZombieDropLootEvent((int)onZombieDropLoot);
	ZombieInitAfterEvent((int)onZombieInitAfter);
	PVZEvent::RandomZombieDropHelmEvent((int)onRandomZombieDropHelm);
}