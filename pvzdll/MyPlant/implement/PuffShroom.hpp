#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class PuffShroom : public BasePlant
	{
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = plant.ImageX + 60;
			rect->Y = plant.ImageY;
			rect->Width = 550;
			rect->Height = plant.Height;
			return true;
		}
	};
}
