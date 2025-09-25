#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Repeater : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 120;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			int rand_type = Creator::Rand(7);
			if (rand_type > 1)
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
