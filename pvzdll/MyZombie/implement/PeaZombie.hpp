#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class PeaZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetSpecialHeadAnimation();
			model.AssignRenderGroupToPrefix(-1, "awake");
			model.AssignRenderGroupToPrefix(-1, "easter");
			model.AssignRenderGroupToPrefix(-1, "damai");

			if (zombie.FromWave != WAVE_ELITE1)
			{
				zombie.ShieldType = ShieldType::ScreenDoor;
				zombie.ShieldHealth = 1100;
				zombie.ShieldMaxHealth = 1100;
				zombie.AttachShield();
			}
		}
	};
}