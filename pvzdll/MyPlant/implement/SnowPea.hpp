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
		int Reload(MyPlant plant, int shoot_cd)
		{
			plant.FireCount = 0;
			return shoot_cd;
		}
		ProjectileType::ProjectileType OverrideProjectileType(MyPlant plant, int plantweapon)
		{
			if (plant.FireCount == 1)//第一发子弹
			{
				if (Creator::RandFloat(1.0f) < icicle_probability[plant.Level])
				{
					plant.ShootOrProductCountdown = Reload(plant, plant.ShootOrProductInterval);

					return ProjectileType::Icicle;
				}
			}
			return (ProjectileType::ProjectileType)-1;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			plant.FireCount++;
			if (proj.Type == ProjectileType::Icicle)
			{
				proj.MakePiercing(6, 10.0f, 0.0f);
				auto particle = PVZ::CreateParticleSystem(proj.X, proj.Y, 0x61A80, (EffectType::EffectType)0x2D);
				particle.OverrideScale(1.5f);
				PVZ::Memory::Execute(AsmBuilder()
					.push_float(13.0f).push_float(8.0f)
					.mov_reg_imm(REG_EDI, proj.GetBaseAddress()+0x7C)
					.mov_reg_imm(REG_ESI, particle.GetBaseAddress())
					.invoke(0x405600).add_reg_imm(REG_ESP,8)
					.ret()
				);
			}

			return true;
		}
	};
}
