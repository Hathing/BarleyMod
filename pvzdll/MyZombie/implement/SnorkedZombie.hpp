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
	};
}