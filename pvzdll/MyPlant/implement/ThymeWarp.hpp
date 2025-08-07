#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class ThymeWarp : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 4000;
		}
	};
}
