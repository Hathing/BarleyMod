#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class LeftRepeater : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 150;
		}
	};
}
