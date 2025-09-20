#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class DiggerZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.HelmHealth = 2400;
			zombie.HelmMaxHealth = 2400;
			zombie.DiggerLastDigRootColumn = 99;
			//zombie.HelmHealth = 750;
			//zombie.HelmMaxHealth = 750;
		}
	};
}
