#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class ScreenDoorZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetAnimation();
			model.AssignRenderGroupToPrefix(-1, "anim_langan");
		}
	};
}
