#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class PotatoMine : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.AttributeCountdown = 1000;
		}
		void onUpgrade(MyPlant plant)
		{
			return;
		}

		bool TickAbility(MyPlant plant)
		{
			switch (plant.State)
			{
			case PlantState::IDLE:
			{

			}
			break;
			case PlantState::POTATO_SPROUT_OUT:
			case PlantState::POTATO_ARMED:
			{
				if (plant.Level == 5)
				{
					//吸引范围内的僵尸
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
			rect->X = plant.ImageX - 40;
			rect->Width = 160;
			return true;
		}
		
	};
}
