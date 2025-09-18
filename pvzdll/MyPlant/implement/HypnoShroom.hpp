#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class HypnoShroom : public BasePlant
	{
		inline static const int summon_cooldown[6] = { 3000, 3000, 3000, 3000, 3000, 3000 };
		inline static const int helm_health[6] = { 370, 570, 570, 770, 770, 770 };
		void onCreated(MyPlant plant)
		{
			plant.Hp = 600;
			plant.MaxHp = 600;
			plant.AttributeCountdown = 100 + Creator::Rand(summon_cooldown[0] - 100);
			plant.State = PlantState::WAIT;
		}
		bool TickAbility(MyPlant plant)
		{
			switch (plant.State)
			{
			case PlantState::WAIT:
				if (plant.AttributeCountdown == 80)
				{
					MyZombie creep = Creator::CreateZombie(ZombieType::ConeheadZombie, plant.Row, plant.Column);
					creep.HelmHealth = helm_health[plant.Level];
					creep.HelmMaxHealth = helm_health[plant.Level];
					creep.SourceLevel = plant.Level;
					creep.Hypnotize();
					creep.SourceID = plant.GetBaseAddress();
					if (plant.Level == MyPlant::MAX_LEVEL)
					{
						creep.GhostFlameMark = 1;
						creep.ReanimShowPrefix("\0", -1);
						creep.ReanimShowPrefix("guihuo", 0);
						creep.ReanimShowPrefix("wenhao", 0);
					}

					plant.SetAnimation("anim_attack", APA_ONCE_STOP, 12);
					Creator::CreateSound(LowerSoundType::MagnetAttract);
				}
				else if (!plant.AttributeCountdown)
				{
					plant.State = PlantState::HYPNOSHROOM_SUMMONING;
					plant.AttributeCountdown = 80;
				}
				break;
			case PlantState::HYPNOSHROOM_SUMMONING:
				if (!plant.AttributeCountdown)
				{
					plant.PlayIdleAnim(12.0f);
					plant.State = PlantState::WAIT;
					plant.AttributeCountdown = summon_cooldown[plant.Level];
				}
				break;
			}
			
			return false;
		}
	};
}
