#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SpikeRock : public BasePlant
	{
		inline static const int max_health[6] = { 500, 1000, 1000, 1500, 1500, 1500 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
			plant.RelatedPlantID1 = 0;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void SelfHeal(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.Heal(10);
			}
			else
			{
				BasePlant::SelfHeal(plant);
			}
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.Squash || plant.NotExist || plant.Sleeping)
				return true;

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
					creep.SetOwner(plant);
					creep.Level = plant.Level;
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
