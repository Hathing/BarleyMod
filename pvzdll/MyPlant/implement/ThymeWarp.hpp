#pragma once
#include "../PlantAbility.hpp"

static const int interval[6] = { 4000, 3500, 3500, 3000, 3000, 3000 };
static const int max_health[6] = { 300, 300, 600, 600, 1000, 1000 };

namespace PlantAbility
{
	class ThymeWarp : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
