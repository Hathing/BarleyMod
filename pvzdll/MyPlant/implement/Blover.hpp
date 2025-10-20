#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Blover : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 600, 600, 600, 600, 600, 600 };
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void onCreated(MyPlant plant)
		{
			plant.SubClass = 2;
			plant.BloverIsWorking = false;
			plant.BloverIsFevering = false;
			plant.SetMaxHealth(max_health[0]);
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.BloverIsWorking)
			{
				plant.BloverAccelerateTime++;
				if (plant.Level >= 5 && plant.BloverAccelerateTime > 6000)
					plant.BloverIsFevering = true;
			
				if (plant.Hp < plant.MaxHp / 2)
				{
					plant.BloverIsWorking = false;
					plant.BloverIsFevering = false;
					auto anim = plant.GetAnimationPart1();
					anim.SetFramesForLayer("anim_idle");
					anim.LoopType = 0;
					plant.BloverAccelerateTime = 0;
				}
			}
			else
			{
				if (plant.Hp >= plant.MaxHp)
				{
					plant.BloverIsWorking = true;
					auto anim = plant.GetAnimationPart1();
					anim.Play("anim_blow",10,3,18.0f);
				}
			}
			if (plant.BloverIsFevering)
			{
				//暂时用植物发光代替绘制
				if ((plant.BloverAccelerateTime / 5) % 2 == 1)
				{
					plant.Light(999);
				}
				else
				{
					plant.Light(0);
				}
			}

			return true;
		}
		void SelfHeal(MyPlant plant)
		{
			if (plant.BloverIsWorking)
			{
				plant.Hp -= 2;
			}
			else
			{
				plant.Heal(2);
			}
		}
	};
}
