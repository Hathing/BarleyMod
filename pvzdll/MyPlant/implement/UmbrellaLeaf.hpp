#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class UmbrellaLeaf : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 1200, 1250, 1300, 1350, 1400, 1650 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void TickPassive(MyPlant plant)
		{
			auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
			for (auto zombie : zombies)
				if (zombie.Row == plant.Row && (zombie.State == ZombieState::POLE_VALUTING_JUMPPING
						|| zombie.State == ZombieState::POGO_JUMP_ACROSS
						|| zombie.State == ZombieState::DOPHIN_JUMP
						|| zombie.State == ZombieState::SNORKEL_JUMP_IN_THE_POOL
						|| zombie.State == ZombieState::BALLOON_FALLING))
					if (plant.ImageX - zombie.X < 80 && plant.ImageX - zombie.X > 160)
					{
						zombie.Blowaway = 1;
						zombie.Layer = 0x61A80 + zombie.Row * 10000;

						Creator::CreateUpperSound(UpperSoundType::FutureOn);
						plant.CreateEffect();
					}

			return;
		}
	};
}
