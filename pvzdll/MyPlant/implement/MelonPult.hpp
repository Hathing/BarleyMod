#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class MelonPult : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 300;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
		{
			while (Creator::Rand(100) < 90)
			{
				proj.SpecialStack += 1;
			}
			return true;
		}
	};
}
