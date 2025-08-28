#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class GatlingPea : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			return;
		}
		bool TickAbility(MyPlant plant)
		{
			if(plant.AnotherCounter>0)
				plant.AnotherCounter -= 1;
			return true;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
		{
			if (plant.AnotherCounter > 0)
			{
				proj.Motion = MotionType::Slide;
				proj.YSpeed = Creator::RandFloat(1.25f);
			}
			return true;
		}
	};
}
