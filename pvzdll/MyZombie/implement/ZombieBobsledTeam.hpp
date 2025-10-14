#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class ZombieBobsledTeam : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.AttributeCountdown = 100;
		}
	};
}
