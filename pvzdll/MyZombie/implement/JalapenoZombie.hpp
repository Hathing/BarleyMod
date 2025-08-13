#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class JalapenoZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetSpecialHeadAnimation();
			model.AssignRenderGroupToPrefix(-1, "awake");
			model.AssignRenderGroupToPrefix(-1, "easter");
		}
	};
}