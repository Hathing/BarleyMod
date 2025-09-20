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
	};
}
