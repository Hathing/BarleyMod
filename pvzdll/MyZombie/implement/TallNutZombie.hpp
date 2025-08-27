#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class TallNutZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetSpecialHeadAnimation();
			model.AssignRenderGroupToPrefix(-1, "easter");

			zombie.HelmHealth = 4000;
			zombie.HelmMaxHealth = 4000;

			zombie.ShieldType = ShieldType::ScreenDoor;
			zombie.ShieldHealth = 1100;
			zombie.ShieldMaxHealth = 1100;
			zombie.AttachShield();
		}
	};
}