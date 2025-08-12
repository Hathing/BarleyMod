#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class ScaredyShroom : public BasePlant
	{
		void onKill(MyPlant plant, MyZombie zombie)
		{
			plant.KillCount++;
		}
	};
}
