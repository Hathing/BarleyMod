#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class WallNut : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 2000;
			plant.MaxHp = 2000;
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				auto model = plant.GetAnimationPart1();
				if (model.isValid())
				{
					if (plant.Hp * 3 < plant.MaxHp)
						model.SetImageOverride("awaken_kuijia", *((PVZ::Image*)0x6FF048));
					else if (plant.Hp * 3 <= plant.MaxHp * 2)
						model.SetImageOverride("awaken_kuijia", *((PVZ::Image*)0x6FF044));
					else
						model.SetImageOverride("awaken_kuijia", PVZ::Image(0));
					return false;
				}
			}
			return true;
		}
	};
}
