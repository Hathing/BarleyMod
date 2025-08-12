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
		}
	};
}
