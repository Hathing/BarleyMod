#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class GoldMagnet : public BasePlant
	{
		bool TickAbility(MyPlant plant)
		{
			switch (plant.State)
			{
			case PlantState::WAIT:
				plant.EffectiveCountdown--;
				if (!plant.EffectiveCountdown)
				{
					plant.State = PlantState::MAGNETSHROOM_INACTIVE_IDLE;
					plant.EffectiveCountdown = 200;
					
					plant.SetAnimation("anim_attract", 3, plant.ChillCountdown ? 6 : 12);
					Creator::CreateLowerSound(LowerSoundType::MagnetAttract);
				}
				break;
			case PlantState::MAGNETSHROOM_INACTIVE_IDLE:
				plant.EffectiveCountdown--;
				if (!plant.EffectiveCountdown)
				{
					plant.State = PlantState::WAIT;
					plant.EffectiveCountdown = 800;

					plant.PlayIdleAnim(12.0);

					bool AllMaxLevel = plant.Level >= MyPlant::MAX_LEVEL;
					if (AllMaxLevel)
					{
						auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
						for (auto& myplant : plants)
							if (myplant.Row == plant.Row && myplant.Level < MyPlant::MAX_LEVEL)
							{
								AllMaxLevel = false;
								break;
							}
					}

					int current_max_row = plant.GetBoard().LevelScene == SceneType::Fog
						|| plant.GetBoard().LevelScene == SceneType::Pool ? 6 : 5;

					if (AllMaxLevel) // 排斥僵尸
					{
						auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
						for (auto& myzombie : zombies)
							if (myzombie.Row == plant.Row && myzombie.X >= 450 && Creator::Rand(10) == 0)
							{
								int tmp_row = Creator::Rand(current_max_row);
								while (tmp_row == myzombie.Row)
									tmp_row = Creator::Rand(current_max_row);
								myzombie.Layer += (tmp_row - myzombie.Row) * 10000;
								myzombie.Row = tmp_row;
							}
					}
					else // 吸取僵尸
					{
						auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
						for (auto& myzombie : zombies)
							if (myzombie.Row != plant.Row && myzombie.X >= 450
								&& myzombie.BodyHealth + myzombie.HelmHealth + myzombie.ShieldHealth
								< (myzombie.BodyMaxHealth + myzombie.HelmMaxHealth + myzombie.ShieldMaxHealth) / 2
								&& Creator::Rand(20) == 0)
							{
								myzombie.Layer += (plant.Row - myzombie.Row) * 10000;
								myzombie.Row = plant.Row;
							}
					}
				}
				break;
			}
			return false;
		}
	};
}
