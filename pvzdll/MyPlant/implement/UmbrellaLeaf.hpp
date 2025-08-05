#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class UmbrellaLeaf : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 1200;
			plant.MaxHp = 1200;
		}
	};
}
