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
		bool TickAbility(MyZombie zombie)
		{
			if (zombie.ShieldType != ShieldType::ZombieAccessoriesType2None && zombie.ZombieHeight == 0)
			{
				if (zombie.AttributeCountdown == 1)
				{
					zombie.StopEating();
					zombie.State = ZombieState::LADDER_PLACING;
					zombie.PlayAnim("anim_placeladder", 24.0f, 10, 3);
				}
			}

			return true;
		}
	};
}