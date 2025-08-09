#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SnowPea : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 180;
		}
	};
}
