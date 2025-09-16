#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SplitPea : public BasePlant
	{
		inline static const int interval[6] = { 150, 120, 120, 100, 100, 100 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			if (secondary)
			{
				rect->X = 0;
				rect->Y = plant.ImageY;
				rect->Width = 1000;
				rect->Height = plant.Height;
			}
			return secondary;
		}
	};
}
