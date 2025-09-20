#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class NewspaperZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			if (zombie.FromWave >= WAVE_ELITE_MASK)
			{
				zombie.BodyHealth = 360;
				zombie.BodyMaxHealth = 360;
			}
		}
	};
}
