#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Torchwood : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 145;
		}
	};
}
