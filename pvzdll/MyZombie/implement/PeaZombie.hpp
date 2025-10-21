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

			if (!zombie.VariantType)
			{
				auto anim = zombie.GetAnimation();
				anim.AssignRenderGroupToPrefix(0, "anim_bucket");
				anim.AssignRenderGroupToPrefix(-1, "anim_hair");
				zombie.HelmType = HelmType::Bucket;
				zombie.HelmHealth = 1100;
				zombie.HelmMaxHealth = 1100;
			}
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