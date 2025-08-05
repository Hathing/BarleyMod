#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Repeater : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 120;
		}
	};
}
