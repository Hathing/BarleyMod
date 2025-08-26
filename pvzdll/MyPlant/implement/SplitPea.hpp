#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SplitPea : public BasePlant
	{
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
