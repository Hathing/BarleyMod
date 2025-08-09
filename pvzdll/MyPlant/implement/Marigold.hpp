#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Marigold : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 3000;
		}
	};
}
