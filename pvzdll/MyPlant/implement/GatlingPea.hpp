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

		bool onUpdateShooting(MyPlant plant)
		{
			if (plant.AnotherCounter > 0)
			{
				plant.Fire(0, 0);
			}
			else
			{
				if (plant.ShootingCountdown == 16 || plant.ShootingCountdown == 30 || plant.ShootingCountdown == 44 || plant.ShootingCountdown == 58 || plant.ShootingCountdown == 72)
				{
					plant.Fire(0, 0);
				}

			}
			plant.ShootingCountdown -= 1;
			if (plant.ShootingCountdown == 0)
			{
				auto anim1 = plant.GetAnimationPart1();
				auto anim2 = plant.GetAnimationPart2();
				if (anim2.isValid() && plant.ShootOrProductInterval > 0)
				{
					StartBlend(20, anim2);
					anim2.SetFramesForLayer("anim_head_idle");
					int base_addr = anim2.GetBaseAddress();
					Memory::WriteMemoryUnsafe<int>((DWORD)base_addr + 0x10, 0);
					anim2.CycleRate = anim1.CycleRate;
					anim2.Speed = anim1.Speed;
				}
				else if (anim1.isValid() && plant.ShootOrProductInterval > 0)
				{
					plant.PlayIdleAnim(anim1.Speed);
				}
				else plant.ShootingCountdown = 1;
			}
			return false;
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
