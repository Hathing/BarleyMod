#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Threepeater : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 145;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
		{
			proj.SpecialFlags = PSF_THREEPEATER_SLIDE_OUT;
			return true;
		}
	};
}
