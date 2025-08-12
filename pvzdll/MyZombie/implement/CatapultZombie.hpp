#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class CatapultZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 650;
			zombie.BodyMaxHealth = 650;
		}
	};
}
