#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class FumeShroom : public BasePlant
	{
		inline static const int interval[6] = { 130, 120, 120, 100, 100, 100 };
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
			rect->X = plant.ImageX + 60;
			rect->Y = plant.ImageY;
			rect->Width = 999999;
			rect->Height = plant.Height;
			return true;
		}
	};
}
