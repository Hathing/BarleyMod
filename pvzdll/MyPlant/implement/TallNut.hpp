#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class TallNut : public BasePlant
	{
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				int partition = plant.Hp * 3 / plant.MaxHp;
				auto model = plant.GetAnimationPart1();
				switch (partition)
				{
				case 0:
					model.SetImageOverride("awaken_2", *((PVZ::Image*)0x6FF0B8));
					break;
				case 1:
					model.SetImageOverride("awaken_2", *((PVZ::Image*)0x6FF0B8));
					break;
				case 2:
				default:
					model.SetImageOverride("awaken_2", PVZ::Image(0));
				}
				return false;
			}
			return true;
		}
	};
}
