#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class KernelPult : public BasePlant
	{
		inline static const int max_health[6] = { 300, 600, 600, 1000, 1000, 1000 };
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			if (proj.Type == ProjectileType::Butter && plant.Level == MyPlant::MAX_LEVEL && plant.Hp >= 800)
			{
				plant.Hp -= 50;
				proj.SpecialStack = 1;
			}
			else
				proj.BounceCount = 5;

			return true;
		}
		int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return int(PVZ::DRF_FLYING | PVZ::DRF_GROUND | PVZ::DRF_SUBMERGED);
		}
	};
}
