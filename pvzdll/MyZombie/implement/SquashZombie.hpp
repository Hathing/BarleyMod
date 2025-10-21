#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class SquashZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetSpecialHeadAnimation();
			if (zombie.FromWave != WAVE_ELITE1)
				model.AssignRenderGroupToPrefix(-1, "awake");
			else
				zombie.InvulnerableDuration = 302;

			model.AssignRenderGroupToPrefix(-1, "easter");
			model.AssignRenderGroupToPrefix(-1, "special");
		}
	};
}
