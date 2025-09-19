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
			rect->Width = 200;
			rect->Height = plant.Height;
			return true;
		}
	};
}
