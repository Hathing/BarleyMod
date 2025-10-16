#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class FootballZombie : public BaseZombie
	{
	public:
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x66 : type;
		}
	};
}
