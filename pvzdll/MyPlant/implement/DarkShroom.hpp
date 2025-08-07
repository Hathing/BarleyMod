#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class DarkShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 70;
		}
	};
}
