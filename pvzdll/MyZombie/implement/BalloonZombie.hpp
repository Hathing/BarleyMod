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
	};
}