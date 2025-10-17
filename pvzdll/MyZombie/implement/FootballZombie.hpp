#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class FootballZombie : public BaseZombie
	{
	public:
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x66 : type;
		}
		bool OverrideDropArmParticle(MyZombie zombie, PVZ::TodParticleSystem particle_sys)
		{
			if (zombie.FromWave == WAVE_ELITE1)
			{
				particle_sys.OverrideImage(*((PVZ::Image*)0x6FF01C));
				return false;
			}
			return true;
		}
		void OverrideDropHelmParticle(MyZombie zombie, PVZ::TodParticleSystem particle_sys)
		{
			if (zombie.FromWave == WAVE_ELITE1)
				particle_sys.OverrideImage(*((PVZ::Image*)0x6FF018));
		}
		bool OverrideHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
		{
			if (zombie.FromWave == WAVE_ELITE1)
			{
				switch (partition)
				{
				case 1:
					reanim.SetImageOverride("zombie_football_helmet", *((PVZ::Image*)0x6FF014));
					return false;
				case 2:
					reanim.SetImageOverride("zombie_football_helmet", *((PVZ::Image*)0x6FF018));
					return false;
				default:
					reanim.SetImageOverride("zombie_football_helmet", *((PVZ::Image*)0x6FF01C));
					return false;
				}
			}
			return true;
		}
	};
}
