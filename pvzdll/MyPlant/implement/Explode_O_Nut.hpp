#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Explode_O_Nut : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
		}
	};
}
