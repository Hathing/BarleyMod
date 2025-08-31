#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class ClownZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.IsWalkingBackwards = 1;
		}
	};
}
