#pragma once
#include "../PlantAbility.hpp"

static const int max_health[6] = { 800, 1000, 1200, 1500, 1500, 1500 };

namespace PlantAbility
{
	class GloomShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = max_health[0];
			plant.MaxHp = max_health[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
	};
}
