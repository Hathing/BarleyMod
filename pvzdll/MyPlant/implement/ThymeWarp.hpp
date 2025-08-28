#pragma once
#include "../PlantAbility.hpp"
#include "NoEasterSkinPlant.hpp"

namespace PlantAbility
{
	class ThymeWarp : public NoEasterSkinPlant
	{
	public:
		inline static const int interval[6] = { 4000, 3500, 3500, 3000, 3000, 3000 };
		inline static const int max_health[6] = { 300, 300, 600, 600, 1000, 1000 };
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
