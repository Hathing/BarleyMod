#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class CabbagePult : public BasePlant
	{
		inline static const int interval[6] = { 500, 480, 480, 460, 460, 460 };
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
