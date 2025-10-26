#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SplitPea : public BasePlant
	{
		inline static const int interval[6] = { 150, 120, 120, 100, 100, 100 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			//仅修改向后发射的索敌矩形
			if (secondary)
			{
				rect->X = 0;
				rect->Y = plant.ImageY;
				rect->Width = 1000;
				rect->Height = plant.Height;
			}
			return secondary;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			if (PlantWeapon == 0)
			{
				//proj.SpecialType = PST_CRACK_PEA;
				if (plant.AnotherCounter > 0)
				{
					//锁血、高亮
					plant.Hp = plant.MaxHp;
					plant.Light();
					//生成子弹
					int x = proj.X, y = proj.Y;
					MyProjectile newproj1{ Creator::CreateProjectile(ProjectileType::Pea,x-5,y,0.0f,2.0f) };
					newproj1.DeriveProperty(proj);
					newproj1.Motion = MotionType::Slide;
					newproj1.YSpeed = -0.75f;
					//newproj1.SpecialType = PST_CRACK_PEA;
					MyProjectile newproj2{ Creator::CreateProjectile(ProjectileType::Pea,x-5,y,0.0f,2.0f) };
					newproj2.DeriveProperty(proj);
					newproj2.Motion = MotionType::Slide;
					newproj2.YSpeed = 0.75f;
					//newproj2.SpecialType = PST_CRACK_PEA;
					MyProjectile newproj3{ Creator::CreateProjectile(ProjectileType::Pea,x-15,y,0.0f,2.0f) };
					newproj3.DeriveProperty(proj);
					newproj3.Motion = MotionType::Slide;
					newproj3.YSpeed = -1.5f;
					//newproj3.SpecialType = PST_CRACK_PEA;
					MyProjectile newproj4{ Creator::CreateProjectile(ProjectileType::Pea,x-15,y,0.0f,2.0f) };
					newproj4.DeriveProperty(proj);
					newproj4.Motion = MotionType::Slide;
					newproj4.YSpeed = 1.5f;
					//newproj4.SpecialType = PST_CRACK_PEA;
				}
			}
			else
			{
				proj.SpecialType = PST_CRACK_PEA;
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
			if (plant.UltraCount != 0)
			{
				plant.UltraCount = 0;
				plant.AnotherCounter = 440;//大招时长
			}
			return true;
		}
		bool onUpdateShooter(MyPlant plant)
		{
			return true;
		}
	};
}
