#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Repeater : public BasePlant
	{
		inline static const int proj_type[6] = { 3,4,5,6,7,7 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 150;
		}
		bool onUpdateShooter(MyPlant plant)
		{
			if (plant.Level >= 5)
			{
				plant.ShootOrProductCountdown -= 1;
				if (plant.ShootOrProductCountdown <= 0)
				{
					int rand_type = Creator::Rand(proj_type[5]);
					if (((1 << rand_type) & plant.ReapeaterRecord) == 0)
					{
						plant.RepeaterNextProjType = rand_type;
						plant.ReapeaterRecord = plant.ReapeaterRecord | 1 << rand_type;
						plant.ShootOrProductCountdown = 25;
					}
					else
					{
						plant.ReapeaterRecord = 0;
						//reload事件
						plant.ShootOrProductCountdown = plant.ShootOrProductInterval - Creator::Rand(15);
					}
					plant.FindTargetAndFire(plant.Row, 0);
				}
				return false;
			}
			return true;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			int rand_type = plant.RepeaterNextProjType;
			if (plant.Level < 5)
			{
				rand_type = Creator::Rand(proj_type[plant.Level]);
			}
			if (rand_type == 1)
			{
				/*
				MyProjectile newproj = Creator::CreateProjectile(ProjectileType::SnowPea,proj.Row,proj.X);
				newproj.DeriveProperty(proj);
				newproj.X = proj.X;
				newproj.Y = proj.Y;
				proj.Remove();
				*/
				proj.Type = ProjectileType::SnowPea;
			}
			else if (rand_type > 1)
			{
				switch (rand_type)
				{
				case 3:
					proj.SpecialType = PST_ORANGE_FIREBALL;
					break;
				case 4:
					proj.SpecialType = PST_BLUE_FIREBALL;
					break;
				case 5:
					proj.SpecialType = PST_PURPLE_FIREBALL;
					break;
				case 6:
					proj.SpecialType = PST_RED_FIREBALL;
					break;
				default:
					break;
				}
				proj.OnFire();
			}
			return true;
		}
		void onEnableEasterSkin(MyPlant plant)
		{
			auto model = plant.GetAnimationPart1();
			if (model.isValid())
			{
				model.AssignRenderGroupToPrefix(1, "backleaf");
				model.AssignRenderGroupToPrefix(4, "awaken_3");
			}
		}
	};
}
