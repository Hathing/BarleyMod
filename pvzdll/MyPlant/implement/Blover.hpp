#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Blover : public BasePlant
	{
		inline static const int max_health[6] = { 300, 450, 450, 600, 600, 600 };
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
