#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class ScaredyShroom : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ScaredyShroomKillCount = 0;
		}
		void onKill(MyPlant plant, MyZombie zombie)
		{
			plant.ScaredyShroomKillCount++;
		}
	};
}
