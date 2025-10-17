#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class FlagZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			if (zombie.FromWave == WAVE_ELITE1)
				zombie.InvulnerableDuration = 502;
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x0A1 : type;
		}
	};
}
