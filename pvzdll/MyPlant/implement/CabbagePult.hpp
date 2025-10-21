#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class CabbagePult : public NoEasterSkinPlant
	{
		inline static const int interval[6] = { 500, 480, 480, 460, 460, 460 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return int(PVZ::DRF_FLYING | PVZ::DRF_GROUND | PVZ::DRF_SUBMERGED);
		}
	};
}
