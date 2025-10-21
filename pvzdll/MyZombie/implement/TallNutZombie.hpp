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
		bool OverrideHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
		{
			auto head = zombie.GetSpecialHeadAnimation();
			switch (partition)
			{
			case 1:
				head.SetImageOverride("common_idle", *((PVZ::Image*)0x6A7200));
				return false;
			case 2:
				head.SetImageOverride("common_idle", *((PVZ::Image*)0x6A77D8));
				return false;
			default:
				return false;
			}
			return false;
		}
	};
}