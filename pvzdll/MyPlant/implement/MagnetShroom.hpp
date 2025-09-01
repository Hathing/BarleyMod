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
		bool TickAbility(MyPlant plant)
		{
			//索敌X最小的僵尸
			if (!plant.NotExist && !plant.Squash)
			{
				auto item = plant.GetMagnetItem(0);
				if (item.Type == MagnetItemType::None)
				{
					plant.MagnetTarget = 0;
				}
				else
				{
					float x = 760.0f;
					auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
					int targetid = 0;
					for (auto& zombie : zombies)
					{
						if (!zombie.Hypnotized && !zombie.NotExist && zombie.NotDying && zombie.Row == plant.Row && zombie.X < x)
						{
							x = zombie.X;
							targetid = zombie.GetBaseAddress();
						}
					}
					plant.MagnetTarget = targetid;
				}
			}
			return true;
		}
	};
}
