#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Threepeater : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 145;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
		{
			proj.SpecialFlags = PSF_THREEPEATER_SLIDE_OUT;
			//新正弦运动的标记
			proj.OriginalY = proj.Y;
			return true;
		}
	};
}
