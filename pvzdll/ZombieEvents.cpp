#include "pch.h"
#include "MyZombie/ZombieAbility.hpp"

void onZombieDropLoot(MyZombie zombie)
{
	zombie.DroppedLoot = 1;
	
	if (zombie.PoisonStack >= 10)
	{
		int spread_stack = zombie.PoisonStack >> 2;
		auto zombies = zombie.GetBoard().GetAllZombies<MyZombie>();
		for (auto myzombie : zombies)
			if (myzombie.Row == zombie.Row && myzombie.X - 80 >= zombie.X && myzombie.X + 80 <= zombie.X)
				myzombie.PoisonStack += spread_stack;
	}
}

void onZombieInitAfter(MyZombie zombie)
{
	ZombieAbility::GetAbility(zombie.Type)->onCreated(zombie);
}

void InitZombieEvents()
{
	ZombieDropLootEvent((int)onZombieDropLoot);
	ZombieInitAfterEvent((int)onZombieInitAfter);
}