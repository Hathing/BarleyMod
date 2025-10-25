#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class DarkShroom : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 70;
		}
		bool onUpdateShooter(MyPlant plant)
		{
			if (plant.ShootOrProductCountdown == 10 || plant.ShootOrProductCountdown == 20)
				plant.FindTargetAndFire(plant.Row, 0);
			return true;
		}
	};
}
