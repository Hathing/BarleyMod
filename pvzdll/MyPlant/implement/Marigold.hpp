#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Marigold : public NoEasterSkinPlant
	{
		inline static const int interval[6] = { 3000, 3000, 2500, 2500, 2000, 2000 };
		inline static const int max_health[6] = { 300, 450, 450, 600, 600, 600 };
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
