#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SpringEquinoxVine : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductCountdown = 0;
			plant.ShootOrProductInterval = 0;
		}
	};
}
