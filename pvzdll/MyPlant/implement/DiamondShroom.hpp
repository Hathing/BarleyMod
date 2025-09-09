#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class DiamondShroom : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 300, 400, 400, 500, 500, 500 };
		void onCreated(MyPlant plant)
		{
			plant.ImageY -= 20;
			plant.ShootOrProductInterval = 300;
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void onKill(MyPlant plant, MyZombie zombie)
		{
			plant.Heal(25);
		}
	};
}
