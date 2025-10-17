#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class DolphinRiderZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 1100;
			zombie.BodyMaxHealth = 1100;
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x92 : type;
		}
	};
}
