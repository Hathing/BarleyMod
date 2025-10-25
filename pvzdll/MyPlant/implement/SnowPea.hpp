#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SnowPea : public BasePlant
	{
		inline static const int interval[6] = { 180, 180, 150, 150, 120, 120 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		bool onUpdateShooter(MyPlant plant)
		{
			if (plant.ShootOrProductCountdown == 40)
				plant.FindTargetAndFire(plant.Row, 0);
			return true;
		}
	};
}
