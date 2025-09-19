#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class TangleKelp : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			return;
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = plant.ImageX - 80;
			rect->Y = plant.ImageY;
			rect->Width = 400;
			rect->Height = plant.Height;
			return true;
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.AnotherCounter > 0)
			{
				plant.AnotherCounter -= 1;
				//技能倒计时>0时，跳过原版更新，包括索敌、抓住僵尸时对目标的处理。
				return false;
			}
			if (plant.TangleKelpDoEffectCountdown > 0)
				plant.TangleKelpDoEffectCountdown -= 1;
			return true;
		}
	};
}
