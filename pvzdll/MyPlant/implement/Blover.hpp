#pragma once
#include "../PlantAbility.hpp"

static const int max_health[6] = { 300, 450, 450, 600, 600, 600 };

namespace PlantAbility
{
	class Blover : public BasePlant
	{
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
