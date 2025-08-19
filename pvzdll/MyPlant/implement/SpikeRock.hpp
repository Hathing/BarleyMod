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
			if (plant.Squash || plant.NotExist || plant.Sleeping)
				return true;

			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.HealCounter++;
				if (plant.HealCounter >= 10)
				{
					plant.HealCounter = 0;
					plant.Heal(1);
				}
			}

			plant.AnotherCounter--;
			auto creep = MyPlant::GetByID(plant.RelatedPlantID1);
			if (creep.isValid() && !creep.Squash)
			{
				creep.ImageX++;
				if (creep.ImageX < 420)
					creep.ImageX++;
				if (creep.ImageX >= 800)
				{
					creep.Remove();
					plant.RelatedPlantID1 = 0;
				}
			}
			else
			{
				if (plant.AnotherCounter <= 0)
				{
					MyPlant creep = Creator::CreatePlant(SeedType::Spickweed, plant.Row, plant.Column);
					plant.AnotherCounter = 800;
					plant.RelatedPlantID1 = creep.Id;
					creep.OwnerID = plant.Id;
					if (plant.EasterSkin)
						creep.EnableEasterSkin();
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
