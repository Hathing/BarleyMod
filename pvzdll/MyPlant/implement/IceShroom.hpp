#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class IceShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 600;
			plant.MaxHp = 600;
		}
	};
}
