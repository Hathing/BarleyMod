#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class RandomZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			if (zombie.FromWave == WAVE_ELITE1)
			{
				zombie.HelmHealth = 1100;
				zombie.HelmMaxHealth = 1100;
			}
			zombie.BodyHealth = 1799;
			zombie.BodyMaxHealth = 1799;
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x93 : type;
		}
		bool OverrideHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
		{
			if (zombie.FromWave == WAVE_ELITE1)
			{
				switch (partition)
				{
				case 1:
					reanim.SetImageOverride("anim_cone", *((PVZ::Image*)0x6FF0CC));
					return false;
				case 2:
					reanim.SetImageOverride("anim_cone", *((PVZ::Image*)0x6FF0D0));
					return false;
				default:
					return false;
				}
			}
			return true;
		}
	};
}
