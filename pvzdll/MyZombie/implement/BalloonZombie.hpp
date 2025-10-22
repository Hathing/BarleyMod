#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class BalloonZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			if(zombie.FromWave >= WAVE_ELITE_MASK)
			{
				zombie.BodyHealth = 300;
				zombie.BodyMaxHealth = 300;
				zombie.FlyingHealth = 800;
				zombie.FlyingMaxHealth = 800;
			}
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x95 : type;
		}
		bool TickAbility(MyZombie zombie)
		{
			if (zombie.State == ZombieState::BALLOON_FLYING)
			{
				//气球自动爆炸
				if ((zombie.X < 316.0f) || bool(zombie.Hypnotized))
				{
					//Call LandFlyer
					PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EAX, zombie.GetBaseAddress()).push_imm32(0).invoke(0x525B60).ret());
				}
			}
			return true;
		}
	};
}