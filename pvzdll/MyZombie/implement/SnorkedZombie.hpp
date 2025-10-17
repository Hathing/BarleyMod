#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class SnorkedZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			if (zombie.GetLawnApp().LevelId != PVZLevel::Zombiguarium)
			{
				zombie.HelmHealth = 540;
				zombie.HelmMaxHealth = 540;
				zombie.HelmType = HelmType::FootballCap;
				zombie.GetAnimation().SetImageOverride((char*)0x668ED4, *((PVZ::Image*)0x6FF024));
			}
		}
		void OverrideDropHelmParticle(MyZombie zombie, PVZ::TodParticleSystem particle_sys)
		{
			particle_sys.OverrideImage(*((PVZ::Image*)0x6FF02C));
		}
		bool OverrideHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
		{
			switch (partition)
			{
			case 1:
				reanim.SetImageOverride("zombie_football_helmet", *((PVZ::Image*)0x6FF028));
				return false;
			case 2:
				reanim.SetImageOverride("zombie_football_helmet", *((PVZ::Image*)0x6FF02C));
				return false;
			default:
				return false;
			}
			return false;
		}
	};
}