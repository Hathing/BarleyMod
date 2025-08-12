#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class DancingZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 1350;
			zombie.BodyMaxHealth = 1350;
		}
	};
}
