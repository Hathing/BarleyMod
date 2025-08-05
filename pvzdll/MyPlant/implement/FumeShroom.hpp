#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class FumeShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 150;
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
