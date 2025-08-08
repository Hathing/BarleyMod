#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Endoflame : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 240;
		}
	};
}
