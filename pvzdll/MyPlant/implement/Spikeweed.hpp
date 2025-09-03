#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Spikeweed : public NoXPPlant
	{
		void OverwritePZDamage(PZDamageEvent* info)
		{
			if (info->plant.Level == MyPlant::MAX_LEVEL)
				info->zombie.AddPoison(1);
		}
	};
}
