#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"
#include <cmath>

namespace PlantAbility
{
	class Threepeater : public NoEasterSkinPlant
	{
		inline static const int interval[6] = { 180, 180, 150, 150, 120, 120 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			//新正弦运动的标记
			proj.OriginalY = proj.Y;
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
				if (shootingcd % 3 == 0)
				{
					int x = plant.ImageX + 50, y = plant.ImageY + 25;
					float rad = 1.14f * std::sinf(shootingcd * 0.02856f );
					float cos = std::cosf(rad);
					float sin = std::sinf(rad);
					constexpr float v = 4.8f;
					MyProjectile newproj1{ Creator::CreateProjectile(ProjectileType::Pea,x,y,0.0f,2.0f) };
					plant.InitAddProjectile(newproj1);
					newproj1.Motion = MotionType::Float;
					newproj1.XSpeed = v;
					newproj1.YSpeed = 0.0f;
					MyProjectile newproj2{ Creator::CreateProjectile(ProjectileType::Pea,x,y,0.0f,2.0f) };
					plant.InitAddProjectile(newproj2);
					newproj2.Motion = MotionType::Float;
					newproj2.XSpeed = v * cos;
					newproj2.YSpeed = v * sin;
					MyProjectile newproj3{ Creator::CreateProjectile(ProjectileType::Pea,x,y,0.0f,2.0f) };
					plant.InitAddProjectile(newproj3);
					newproj3.Motion = MotionType::Float;
					newproj3.XSpeed = v * cos;
					newproj3.YSpeed = v * sin * -1;
				}
				//关闭大招状态
				if (shootingcd < 3)
					plant.AnotherCounter = 0;
			}
			return true;
		}
	};
}
