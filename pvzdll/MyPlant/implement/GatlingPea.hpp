#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class GatlingPea : public BasePlant
	{
		inline static const int interval[6] = { 150, 120, 120, 100, 100, 100 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
		}
		bool TickAbility(MyPlant plant)
		{
			if(plant.AnotherCounter>0)
				plant.AnotherCounter -= 1;
			return true;
		}

		bool onUpdateShooting(MyPlant plant)
		{
			if (plant.UltraCount != 0)
			{
				plant.UltraCount = 0;
				int ultra_time = 300;
				plant.AnotherCounter = ultra_time;
				plant.ShootingCountdown = plant.AnotherCounter;
				plant.ShootOrProductCountdown += plant.AnotherCounter;
			}

			if (plant.AnotherCounter > 0)
			{
				plant.Light();
				if (plant.ShootingCountdown % 2 == 1)
					plant.Fire(0, 0);
			}
			else
			{
				switch (plant.Level)
				{
				case 0:
				case 1:
					{
						switch (plant.ShootingCountdown)
						{
						case 68:
						case 51:
						case 35:
						case 18:
							plant.Fire(0,0);
							break;
						default:
							break;
						}
					}
					break;
				case 2:
				case 3:
					{
						switch (plant.ShootingCountdown)
						{
						case 72:
						case 58:
						case 45:
						case 31:
						case 18:
							plant.Fire(0, 0);
							break;
						default:
							break;
						}
					}
					break;
				case 4:
				case 5:
					{
						switch (plant.ShootingCountdown)
						{
						case 75:
						case 63:
						case 52:
						case 40:
						case 29:
						case 17:
							plant.Fire(0, 0);
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
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
					Memory::WriteMemory<int>((DWORD)base_addr + 0x10, 0);//播放类型为循环
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

		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
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
