#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SpikeRock : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
			plant.RelatedPlantID1 = 0;
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.HealCounter++;
				if (plant.HealCounter >= 10)
				{
					plant.HealCounter = 0;
					plant.Heal(1);
				}
			}
			return true;
		}
		void onDie(MyPlant plant)
		{
			if (plant.OnBoard)
			{
				auto creep = MyPlant::GetByID(plant.RelatedPlantID1);
				if (creep.isValid())
					creep.Remove();
			}
		}
	};
}
