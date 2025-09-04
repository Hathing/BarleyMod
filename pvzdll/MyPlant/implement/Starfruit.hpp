#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Starfruit : public BasePlant
	{
		inline static const int interval[6] = { 150, 135, 135, 120, 120, 120 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
	};
}
