#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Mangosteen : public BasePlant
	{
	public:
		inline static const int max_health[6] = { 300, 400, 400, 500, 500, 500 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 130;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		int GetEasterProbabilityPartition()
		{
			return 1;
		}
		bool onUpdateShooter(MyPlant plant)
		{
			if (plant.ShootOrProductCountdown == 15 || plant.ShootOrProductCountdown == 30
					|| plant.ShootOrProductCountdown == 45 || plant.ShootOrProductCountdown == 60)
				plant.FindTargetAndFire(plant.Row, 0);
			return true;
		}
	};
}
