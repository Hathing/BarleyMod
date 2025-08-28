#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class ScaredyShroom : public NoEasterSkinPlant
	{
		void onKill(MyPlant plant, MyZombie zombie)
		{
			plant.KillCount++;
		}
	};
}
