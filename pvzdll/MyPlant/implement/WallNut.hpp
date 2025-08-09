#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class WallNut : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 2000;
			plant.MaxHp = 2000;
		}
	};
}
