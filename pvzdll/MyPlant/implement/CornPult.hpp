#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class CornPult : public BasePlant
	{
		inline static const int max_health[6] = { 300, 600, 600, 1000, 1000, 1000 };
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
