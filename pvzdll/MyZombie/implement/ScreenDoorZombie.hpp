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
	};
}
