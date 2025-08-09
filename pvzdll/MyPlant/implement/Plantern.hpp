#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Plantern : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 4000;
			plant.MaxHp = 4000;
			plant.RespawnType = SeedType::None;
		}
	};
}
