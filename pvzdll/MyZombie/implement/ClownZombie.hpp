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
		bool TickAbility(MyZombie zombie)
		{
			if (zombie.State == ZombieState::JACKBOX_WALKING
				&& ((!zombie.Hypnotized && zombie.IsWalkingBackwards == 0) || (zombie.Hypnotized && zombie.X > 725.0f)))
			{
				zombie.AttributeCountdown = 0;
			}
			return true;
		}
	};
}
