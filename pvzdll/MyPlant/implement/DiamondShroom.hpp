#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	inline static const int max_health[6] = { 300, 400, 400, 500, 500, 500 };
	class DiamondShroom : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ImageY -= 20;
			plant.ShootOrProductInterval = 300;
		}
		void onKill(MyPlant plant, MyZombie zombie)
		{
			plant.Heal(25);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
