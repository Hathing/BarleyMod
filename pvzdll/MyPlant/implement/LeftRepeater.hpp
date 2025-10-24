#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class LeftRepeater : public BasePlant
	{
		inline static const int shoot_cd[6] = { 150,120,120,100,100,100 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = shoot_cd[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = shoot_cd[plant.Level];
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = 0;
			rect->Y = plant.ImageY;
			rect->Width = 1000;
			rect->Height = plant.Height;
			return true;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{

			switch (plant.Level)
			{
			case 2:
			case 3:
				proj.SpecialType = PST_ORANGE_FIREBALL;
				break;
			case 4:
			case 5:
				proj.SpecialType = PST_RED_FIREBALL;
				break;
			default:
				break;
			}

			if (plant.Level >= 5)
			{
				if (Creator::RandFloat(1.0f) < 0.1f)
				{
					proj.SpecialType = PST_BLACK_FIREBALL;
				}
			}

			proj.OnFire();

			return true;
		}
	};
}
