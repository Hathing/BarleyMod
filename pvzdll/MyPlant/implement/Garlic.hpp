#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Garlic : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 400;
			plant.MaxHp = 400;
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				int partition = plant.Hp * 3 / plant.MaxHp;
				auto model = plant.GetAnimationPart1();
				switch (partition)
				{
				case 0:
					model.SetImageOverride("awaken_face", *((PVZ::Image*)0x6FF0F0));
					break;
				case 1:
					model.SetImageOverride("awaken_face", *((PVZ::Image*)0x6FF0EC));
					break;
				case 2:
				default:
					model.SetImageOverride("awaken_face", PVZ::Image(0));
				}
				return false;
			}
			return true;
		}
	};
}
