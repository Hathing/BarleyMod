#pragma once
#include "../PlantAbility.hpp"
#include "NoEasterSkinPlant.hpp"

namespace PlantAbility
{
	class DarkShroom : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 70;
		}
	};
}
