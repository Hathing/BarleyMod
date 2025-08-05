#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class CabbagePult : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 500;
		}
	};
}
