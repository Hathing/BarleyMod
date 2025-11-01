#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Cactus : public BasePlant
	{
		inline static const int max_health[6] = { 500, 1000, 1500, 1500, 1500, 1500 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
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
		bool onSquishedByZombie(MyPlant plant, MyZombie zombie)
		{
			return PVZ::ApplyZPDamage(zombie, plant, 800);
		}
	};
}
