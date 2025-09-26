#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Marigold : public NoEasterSkinPlant
	{
		inline static const int interval[6] = { 3000, 3000, 2500, 2500, 2000, 2000 };
		inline static const int max_health[6] = { 300, 450, 450, 600, 600, 600 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[0];
		}
		void onUpgrade(MyPlant plant)
		{
			plant.ShootOrProductInterval = interval[plant.Level];
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.ShootOrProductCountdown == 1 && plant.MarigoldItemType == 0)
			{
				int itemtype = Creator::Rand(2 + plant.Level / 2);
				switch (itemtype)
				{
				case 0://水壶
					{
						auto target = plant;
						bool priority = (plant.Hp < plant.MaxHp);
						auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
						for (auto& plant_ : plants)
						{
							if (!plant_.NotExist && plant_.Row == plant.Row && !plant_.Squash && plant_.IsXPRecipient() && plant_.IsPrime())
							{
								bool notfull = (plant_.Hp < plant_.MaxHp);
								if (!priority && notfull)
								{
									target = plant_;
								}
								else if ((priority && notfull) || (!priority && !notfull))
								{
									if (plant_.Hp < target.Hp)
									{
										target = plant_;
									}
								}
							}
						}
						plant.MarigoldItemType = 1;
						plant.AttributeCountdown = 100;
						plant.MarigoldTargetID = target.Id;
						Creator::CreateLowerSound(LowerSoundType::Watering);
						auto anim = Creator::CreateReanimation(AnimationType::Kettle, target.ImageX, target.ImageY, target.Layer + 2);
						anim.Play("anim_water_area", 0, 2, 0.0f);
					}
					break;
				case 1://钉耙
					{
						Creator::CreateRake(plant.Row, Creator::Rand(5) + 5);
					}
					break;
				case 2://杀虫剂
					{
						auto anim = Creator::CreateReanimation(AnimationType::Spray, 200.0f, plant.ImageY, plant.Layer + 2);
						anim.Play("anim_spray", 0, 2, 0.0f);
					}
					break;
				case 3://肥料
					{
						auto target = plant;
						bool priority = (plant.Level < 5);
						auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
						for (auto& plant_ : plants)
						{
							if (!plant_.NotExist && plant_.Row == plant.Row && !plant_.Squash && plant_.IsXPRecipient() && plant_.IsPrime())
							{
								bool notfull = (plant_.Level < 5);
								if (!priority && notfull)
								{
									target = plant_;
								}
								else if ((priority && notfull) || (!priority && !notfull))
								{
									if (plant_.Experience > target.Experience)
									{
										target = plant_;
									}
								}
							}
						}
						plant.MarigoldItemType = 4;
						plant.AttributeCountdown = 100;
						plant.MarigoldTargetID = target.Id;
						Creator::CreateLowerSound(LowerSoundType::Fertilize);
						auto anim = Creator::CreateReanimation(AnimationType::TreeofwisdomFertilizer, target.ImageX, target.ImageY, target.Layer + 2);
						PVZ::Memory::WriteMemory<int>(anim.GetBaseAddress() + 0x10, 2);
					}
					break;
				default:
					break;
				}
			}
			//水壶生效
			if (plant.MarigoldItemType == 1 && plant.AttributeCountdown == 1)
			{
				auto target = PVZ::GetByID<MyPlant>(plant.MarigoldTargetID);
				if (target.isValid())
				{
					target.Flash();
					int overwhelm = target.Heal(300);
					if (overwhelm > 0)
					{
						target.AddExperience(overwhelm * 10);
					}
				}
				plant.MarigoldItemType = 0;
			}
			//肥料生效
			else if (plant.MarigoldItemType == 4 && plant.AttributeCountdown == 1)
			{
				auto target = PVZ::GetByID<MyPlant>(plant.MarigoldTargetID);
				if (target.isValid())
				{
					target.FertilizedCounter = 1000;
				}
				plant.MarigoldItemType = 0;
			}
			return true;
		}
	};
}
