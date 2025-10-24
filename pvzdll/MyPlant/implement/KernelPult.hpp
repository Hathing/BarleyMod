#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class KernelPult : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.KernelRandomFire = false;
			plant.ButterHitCount = 0;
			plant.KernelPreparingCannon = false;
		}
		void onUpgrade(MyPlant plant)
		{
			return;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			if (proj.Type == ProjectileType::CobCannon)
			{
				proj.RotationSpeed = -0.2f;
				proj.X -= 35.0f;
				proj.Height -= 80.0f;
			}
			else
			{
				proj.BounceCount = 1;
			}
			/*
			if (proj.Type == ProjectileType::Butter && plant.Level == MyPlant::MAX_LEVEL && plant.Hp >= 800)
			{
				plant.Hp -= 50;
				proj.SpecialStack = 1;
			}	
			*/
			return true;
		}
		int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return int(PVZ::DRF_FLYING | PVZ::DRF_GROUND | PVZ::DRF_SUBMERGED);
		}
	};
}
