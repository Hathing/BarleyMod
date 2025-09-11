#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class TallNut : public BasePlant
	{
		inline static const int max_health[6] = { 1000, 1500, 2000, 3000, 4000, 4000 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void TickPassive(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.HealCounter++;
				if (plant.HealCounter >= 100)
				{
					plant.HealCounter = 0;
					plant.Heal(50);
				}
			}
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				int partition = plant.Hp * 3 / plant.MaxHp;
				auto model = plant.GetAnimationPart1();
				if (model.isValid())
				{
					switch (partition)
					{
					case 0:
						model.SetImageOverride("awaken_2", *((PVZ::Image*)0x6FF0B8));
						break;
					case 1:
						model.SetImageOverride("awaken_2", *((PVZ::Image*)0x6FF0B8));
						break;
					case 2:
					default:
						model.SetImageOverride("awaken_2", PVZ::Image(0));
					}
					return false;
				}
			}
			return true;
		}
	};
}
