#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Squash : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
		}
	};
}
