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
	};
}
