#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class GloomShroom : public BasePlant
	{
		inline static const int max_health[6] = { 800, 1000, 1200, 1500, 1500, 1500 };
		void onCreated(MyPlant plant)
		{
			plant.Hp = max_health[0];
			plant.MaxHp = max_health[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
			if (plant.Level == MyPlant::MAX_LEVEL)
			{
				auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
				for (auto& myplant : plants)
					if (myplant.Row == plant.Row && myplant.Column >= 2 && myplant.Row <= 4
							&& !myplant.Squash && myplant.Id != plant.Id)
						plant.SetMaxHealth(plant.MaxHp + (myplant.MaxHp >> 2));
			}
		}
		void TickPassive(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.HealCounter++;
				if (plant.HealCounter >= 5)
				{
					plant.HealCounter = 0;
					plant.Heal(1);
				}
			}
		}
	};
}
