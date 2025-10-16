#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class Imp : public BaseZombie
	{
	public:
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x99 : type;
		}
	};
}
