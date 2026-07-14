#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Chomper : public BasePlant
	{
		inline static const int max_health[6] = { 500, 1000, 1500, 1500, 1500, 1500 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void OverwritePZDamage(PZDamageEvent* info)
		{
			if (info->type == PVZEvent::PLANTDAMAGETYPE_CHOMPER)
			{
				//啃咬伤害，原版为40/70cs
				info->damage = 600;
			}
		}
	};
}
