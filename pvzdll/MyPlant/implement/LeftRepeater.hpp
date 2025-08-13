#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class LeftRepeater : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 150;
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = 0;
			rect->Y = plant.ImageY;
			rect->Width = 1000;
			rect->Height = plant.Height;
			return true;
		}
	};
}
