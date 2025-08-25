#pragma once
#include "../PlantAbility.hpp"

static const int max_health[6] = { 300, 400, 400, 500, 500, 500 };

namespace PlantAbility
{
	class Mangosteen : public BasePlant
	{
	public:
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 150;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
