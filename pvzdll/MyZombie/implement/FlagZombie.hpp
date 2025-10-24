#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class FlagZombie : public BaseZombie
	{
	public:
		inline static constexpr ZombieType::ZombieType summon_pool[4] = {ZombieType::Undying, ZombieType::BackupDancer,
			ZombieType::DuckyTubeZombie, ZombieType::ZombieBobsledTeam};
		void onCreated(MyZombie zombie)
		{
			zombie.SummonCounter = 100;

			auto model = zombie.GetAnimation();
			switch (zombie.FromWave)
			{
			case 1:
				model.SetImageOverride("Zombie_flag", *((PVZ::Image*)0x6FF0C4));
				break;
			case 2:
				model.SetImageOverride("Zombie_flag", *((PVZ::Image*)0x6FF0BC));
				zombie.InvulnerableDuration = 502;
				break;
			default:
				break;
			}
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x0A1 : type;
		}
		bool TickAbility(MyZombie zombie)
		{
			zombie.SummonCounter--;
			if (!zombie.SummonCounter)
			{
				MyBoard board = zombie.GetBoard();
				board.AddZombieInRow(summon_pool[Creator::Rand(4)], zombie.Row, -1);
			}

			return false;
		}
	};
}
