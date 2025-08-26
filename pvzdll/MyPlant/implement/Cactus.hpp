#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Cactus : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
		}
		bool onAnimate(MyPlant plant)
		{
			if (!plant.EasterSkin)
			{
				auto model = plant.GetAnimationPart1();

				model.AssignRenderGroupToPrefix(plant.Hp * 3 <= plant.MaxHp ? -1 : 0, "cactus_arm1_1");
				model.AssignRenderGroupToPrefix(plant.Hp * 3 <= plant.MaxHp ? -1 : 0, "cactus_arm1_2");
				model.AssignRenderGroupToPrefix(plant.Hp * 3 / 2 <= plant.MaxHp ? -1 : 0, "cactus_arm2_1");
				model.AssignRenderGroupToPrefix(plant.Hp * 3 / 2 <= plant.MaxHp ? -1 : 0, "cactus_arm2_2");
			}
			return false;
		}
	};
}
