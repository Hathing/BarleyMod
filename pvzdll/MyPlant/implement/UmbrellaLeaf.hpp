#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class UmbrellaLeaf : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 1200, 1250, 1300, 1350, 1400, 1650 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
