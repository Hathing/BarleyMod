#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class UmbrellaLeaf : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 1200;
			plant.MaxHp = 1200;
		}
	};
}
