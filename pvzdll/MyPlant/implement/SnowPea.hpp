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
			if (plant.UltraCount != 0)
			{
				return ProjectileType::Icicle;
			}

			return (ProjectileType::ProjectileType)-1;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			plant.FireCount++;
			if (proj.Type == ProjectileType::Icicle)
			{
				InitIcicle(proj, 6, 10.0f, 0.0f);
			}
			return true;
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.AnotherCounter > 0)
				plant.AnotherCounter -= 1;
			return true;
		}
		bool onUpdateShooting(MyPlant plant)
		{
			int shootingcd = plant.ShootingCountdown;
			if (shootingcd > 1 && plant.AnotherCounter > 0)
			{
				//锁血、高亮
				plant.Hp = plant.MaxHp;
				plant.Light();
				//生成子弹
				if (shootingcd % 10 == 0)
				{
					int wave = (shootingcd % 30) / 10;
					int wave_order = 2 - wave;
					int x = plant.ImageX + 60, y = plant.ImageY + 40, row = plant.Row, layer = 0x4A768 + 10000 * row + 100 * wave_order;
					///*

					constexpr int offset_x = 20;
					constexpr int offset_y = 20;
					constexpr float t = 10.0f;
					constexpr float v = 10.0f;
					constexpr float base_offset_v_x = offset_x / t;
					constexpr float base_offset_v_y = offset_y / t;

					float base_v_x = (60.0f - wave_order * 60.0f) / t + v * wave_order;

					MyProjectile newproj_center{ MyCreateProjectile(ProjectileType::Icicle,row,layer,x,y) };
					plant.InitAddProjectile(newproj_center);
					newproj_center.SpecialType = PST_ULTRA_ICICLE;
					InitIcicle(newproj_center, 6, base_v_x, 0.0f);
					layer -= 0x10;

					for (int i = 0; i < wave; i++)
					{
						float v_x = base_v_x - base_offset_v_x * (i + 1);
						float v_y = base_offset_v_y * (i + 1);

						MyProjectile newproj_1{ MyCreateProjectile(ProjectileType::Icicle,row,layer,x,y) };
						plant.InitAddProjectile(newproj_1);
						newproj_1.SpecialType = PST_ULTRA_ICICLE;
						InitIcicle(newproj_1, 6, v_x, v_y);
						MyProjectile newproj_2{ MyCreateProjectile(ProjectileType::Icicle,row,layer,x,y) };
						plant.InitAddProjectile(newproj_2);
						newproj_2.SpecialType = PST_ULTRA_ICICLE;
						InitIcicle(newproj_2, 6, v_x, -1.0 * v_y);

						layer -= 0x10;
					}
					//*/
				}
				//关闭大招状态
				if (shootingcd < 10)
					plant.AnotherCounter = 0;
			}
			return true;
		}

		void InitIcicle(MyProjectile proj,int piercingnum,float xspeed,float yspeed)
		{
			proj.DamageAbility = PVZ::DRF_GROUND;
			proj.MakePiercing(piercingnum, xspeed, yspeed);
			//auto particle = PVZ::CreateParticleSystem(proj.X, proj.Y, 0x61A80, (EffectType::EffectType)0x2D);
			//particle.OverrideScale(1.5f);
			/*
			PVZ::Memory::Execute(AsmBuilder()
				.push_float(13.0f).push_float(8.0f)
				.mov_reg_imm(REG_EDI, proj.GetBaseAddress() + 0x7C)
				.mov_reg_imm(REG_ESI, particle.GetBaseAddress())
				.invoke(0x405600).add_reg_imm(REG_ESP, 8)
				.ret()
			);
			*/
		}
	};
}
