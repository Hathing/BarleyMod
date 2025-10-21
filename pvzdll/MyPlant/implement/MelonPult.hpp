#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"
#include <cassert>

namespace PlantAbility
{
	class MelonPult : public NoEasterSkinPlant
	{
		inline static const int bounce_count[6] = { 0,0,1,1,2,2 };
		inline static const int size_stack[6] = { 2,4,4,6,6,6 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 300;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			proj.BounceCount = bounce_count[plant.Level];
			proj.SpecialStack += size_stack[plant.Level];
			if (plant.Level >= 5)
			{
				while (Creator::Rand(100) < 90)
				{
					proj.SpecialStack += 1;
				}
			}
			return true;
		}
		bool TickAbility(MyPlant plant)
		{
			plant.ShootOrProductInterval = 300;
			assert(plant.ShootOrProductCountdown >= 0);
			return true;
		}
		int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return int(PVZ::DRF_FLYING | PVZ::DRF_GROUND | PVZ::DRF_SUBMERGED);
		}
	};
}
