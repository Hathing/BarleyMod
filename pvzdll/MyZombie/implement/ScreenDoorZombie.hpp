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

			if (zombie.FromWave == WAVE_ELITE1)
			{
				model.AssignRenderGroupToPrefix(0, "anim_bucket");
				model.AssignRenderGroupToPrefix(-1, "anim_hair");
				zombie.HelmType = HelmType::Bucket;
				zombie.HelmHealth = 1100;
				zombie.HelmMaxHealth = 1100;
			}
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			switch (zombie.FromWave)
			{
			case WAVE_ELITE1:
				return (AnimationType::AnimationType)0x93;
			case WAVE_ELITE2:
				return (AnimationType::AnimationType)0x9F;
			default:
				return type;
			}
		}
		bool OverrideHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
		{
			if (zombie.FromWave == WAVE_ELITE1)
			{
				switch (partition)
				{
				case 1:
					reanim.SetImageOverride("anim_cone", *((PVZ::Image*)0x6FF160));
					return false;
				case 2:
					reanim.SetImageOverride("anim_cone", *((PVZ::Image*)0x6FF164));
					return false;
				default:
					return false;
				}
			}
			return true;
		}
	};
}
