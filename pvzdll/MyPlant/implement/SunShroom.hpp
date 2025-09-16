#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class SunShroom : public NoEasterSkinPlant
	{
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = plant.ImageX + 60;
			rect->Y = plant.ImageY;
			rect->Width = 550;
			rect->Height = plant.Height;
			return true;
		}
		virtual bool IsXPRecipient(MyPlant plant)
		{
			return !plant.Squash;
		}
		void onUpgrade(MyPlant plant)
		{
			if (plant.Level == MyPlant::MAX_LEVEL)
				plant.Experience = 0;
		}
	};
}
