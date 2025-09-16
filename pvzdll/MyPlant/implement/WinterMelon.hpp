#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class WinterMelon : public BasePlant
	{
		inline static const int max_health[6] = { 300, 400, 400, 600, 1500, 1500 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
