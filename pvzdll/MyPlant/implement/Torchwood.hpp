#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Torchwood : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 145;
		}
	};
}
