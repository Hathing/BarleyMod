#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class BucketHeadZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			if (Creator::Rand(10) < 5)
			{
				zombie.ShieldType = ShieldType::ScreenDoor;
				zombie.ShieldHealth = 1100;
				zombie.ShieldMaxHealth = 1100;
				zombie.AttachShield();
			}
		}
	};
}