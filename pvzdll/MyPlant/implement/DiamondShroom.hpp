#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class DiamondShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ImageY -= 20;
			plant.ShootOrProductInterval = 300;
		}
	};
}
