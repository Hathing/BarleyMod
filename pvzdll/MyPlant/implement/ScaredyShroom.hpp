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
		int Reload(MyPlant plant, int shoot_cd)
		{
			plant.ShootOrProductInterval -= 5;
			if (plant.ShootOrProductInterval < 50)
				plant.ShootOrProductInterval = 50;
			return shoot_cd;
		}
	};
}
