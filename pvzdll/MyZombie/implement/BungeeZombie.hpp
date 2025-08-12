#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class BungeeZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 200;
			zombie.BodyMaxHealth = 200;
		}
	};
}
