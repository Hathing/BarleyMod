#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class MagnetShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
		}
		void onUpgrade(MyPlant plant)
		{
			switch (plant.Level)
			{
			case 1:
				plant.MaxHp = 1000;
				plant.Hp += 500;
				break;
			case 3:
				plant.MaxHp = 2000;
				plant.Hp += 1000;
				break;
			}
		}
	};
}
