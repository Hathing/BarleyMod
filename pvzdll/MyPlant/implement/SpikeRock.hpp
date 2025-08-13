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
