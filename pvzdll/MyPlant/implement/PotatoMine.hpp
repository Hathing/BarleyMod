#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class PotatoMine : public BasePlant
	{
		inline static const int explode_range[6] = { 120, 120, 160, 160, 200, 200 };
		void onCreated(MyPlant plant)
		{
			plant.AttributeCountdown = 1;
			plant.AnotherCounter = 1000;
		}
		void onUpgrade(MyPlant plant)
		{
			return;
		}

		bool TickAbility(MyPlant plant)
		{
			//本体种小雷
			if (plant.OwnerID == 0)
			{
				if (plant.State == PlantState::POTATO_ARMED)
				{
					if (plant.AnotherCounter > 0)
						plant.AnotherCounter--;
					else
					{
						plant.AnotherCounter = 1000;
						//索敌
						auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
						int target_x = 99999999;
						for (auto& zombie : zombies)
						{
							if (zombie.Row == plant.Row && zombie.EffectedBy(PVZ::DRF_GROUND || PVZ::DRF_UNDERGROUND || PVZ::DRF_SUBMERGED || PVZ::DRF_OFF_GROUND || PVZ::DRF_DYING))
							{
								if (zombie.X < target_x)
								{
									target_x = zombie.X;
								}
							}
						}
						if (target_x < 720)
						{
							auto child = plant.CreateChildPlant();
							child.ImageX = target_x;
						}
						else
						{
							//索敌失败，设置CD为1s
							plant.AnotherCounter = 100;
						}
					}
				}
			}

			switch (plant.State)
			{
			case PlantState::IDLE:
			{

			}
			break;
			case PlantState::POTATO_SPROUT_OUT:
			case PlantState::POTATO_ARMED:
			{
				if (plant.OwnerID != 0 && plant.Level == 5)
				{
					//小雷吸引范围内的僵尸
					PVZ::Rect attack_rect;
					plant.GetPlantAttackRect(0, attack_rect);
					//吸引半径略大于爆炸半径
					attack_rect.X -= 40;
					attack_rect.Width += 80;
					auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
					for (auto& zombie : zombies)
					{
						if (zombie.Row == plant.Row && zombie.EffectedBy(PVZ::DRF_GROUND || PVZ::DRF_UNDERGROUND || PVZ::DRF_SUBMERGED || PVZ::DRF_OFF_GROUND || PVZ::DRF_DYING))
						{
							auto zombie_rect = zombie.GetActualRect();
							if (PVZ::GetXOverlap(zombie_rect, attack_rect) >= 0)
							{
								int center = plant.ImageX;
								zombie.X = center + (zombie.X - center) * 0.95f;
							}
						}
					}
				}
			}
			break;
			default:
				break;
			}
			return true;
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			int radius = explode_range[plant.Level];
			rect->X = plant.ImageX - ((radius - 80) / 2);
			rect->Y = plant.ImageY;
			rect->Width = radius;
			rect->Height = plant.Height;
			return true;
		}
		
	};
}
