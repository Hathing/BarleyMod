#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SnowPea : public BasePlant
	{
		inline static const int interval[6] = { 180, 180, 150, 150, 120, 120 };
		inline static const float icicle_probability[6] = { 0.15f, 0.25f, 0.25f, 0.35f, 0.35f, 0.35f };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		bool onUpdateShooter(MyPlant plant)
		{
			if (plant.ShootOrProductCountdown == 40)
				plant.FindTargetAndFire(plant.Row, 0);
			return true;
		}
		ProjectileType::ProjectileType OverrideProjectileType(MyPlant plant, int plantweapon)
		{
			if (Creator::RandFloat(1.0f) < icicle_probability[plant.Level])
			{
				return ProjectileType::Icicle;
			}
			return (ProjectileType::ProjectileType)-1;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			if (proj.Type == ProjectileType::Icicle)
			{
				proj.MakePiercing(6, 10.0f, 0.0f);
			}

			return true;
		}
	};
}
