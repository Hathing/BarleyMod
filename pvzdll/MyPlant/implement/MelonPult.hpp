#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class MelonPult : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 300;
		}
	};
}
