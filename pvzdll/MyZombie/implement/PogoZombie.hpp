#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class PogoZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 720;
			zombie.BodyMaxHealth = 720;
			if (zombie.FromWave >= WAVE_ELITE_MASK)
			{
				zombie.BodyHealth = 140;
				zombie.BodyMaxHealth = 140;
			}
		}
	};
}
