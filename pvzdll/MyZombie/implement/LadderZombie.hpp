#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class LadderZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			zombie.AttributeCountdown = 500;
		}
	};
}