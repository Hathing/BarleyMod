#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Garlic : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 400, 450, 500, 550, 600, 600 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void TickPassive(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.HealCounter++;
				if (plant.HealCounter >= 100)
				{
					plant.HealCounter = 0;
					plant.Heal(1);
				}
			}
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				int partition = plant.Hp * 3 / plant.MaxHp;
				auto model = plant.GetAnimationPart1();
				switch (partition)
				{
				case 0:
					model.SetImageOverride("awaken_face", *((PVZ::Image*)0x6FF0F0));
					break;
				case 1:
					model.SetImageOverride("awaken_face", *((PVZ::Image*)0x6FF0EC));
					break;
				case 2:
				default:
					model.SetImageOverride("awaken_face", PVZ::Image(0));
				}
				return false;
			}
			return true;
		}
	};
}
