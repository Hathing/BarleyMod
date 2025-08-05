#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Threepeater : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 145;
		}
	};
}
