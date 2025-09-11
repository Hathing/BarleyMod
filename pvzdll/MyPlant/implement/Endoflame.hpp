#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Endoflame : public BasePlant
	{
		inline static const int interval[6] = { 240, 220, 220, 200, 200, 500 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		int GetEasterProbabilityPartition()
		{
			return 1;
		}
		bool onUpdateShooter(MyPlant plant)
		{
			return true;
		}
	};
}
