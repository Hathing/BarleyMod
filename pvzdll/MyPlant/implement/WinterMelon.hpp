#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class WinterMelon : public BasePlant
	{
		inline static const int max_health[6] = { 300, 300, 300, 300, 300, 300 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
		{
			if (plant.WinterMelonCastState == 1)
				proj.SpecialType = PST_SCATTER_WINTERMELON;
			else if (plant.WinterMelonCastState == 2)
			{
				proj.SpecialType = PST_CANNON_WINTERMELON;
				plant.WinterMelonCastState = 0;
			}
			return true;
		}
	};
}
