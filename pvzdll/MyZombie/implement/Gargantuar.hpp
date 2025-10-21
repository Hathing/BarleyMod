#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class Gargantuar : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetAnimation();
			model.AssignRenderGroupToPrefix(-1, "anim_bucket");
			model.AssignRenderGroupToPrefix(-1, "anim_screendoor");
		}
		bool OverrideDrawPos(MyZombie zombie, PVZ::ZombieDrawPosition* draw_pos)
		{
			if (zombie.Hypnotized)
				draw_pos->ImageOffsetX += 30.0f;
			return true;
		}
	};
	class GigaGargantuar : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			return;
		}
		bool OverrideDrawPos(MyZombie zombie, PVZ::ZombieDrawPosition* draw_pos)
		{
			if (zombie.Hypnotized)
				draw_pos->ImageOffsetX += 30.0f;
			return true;
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x0A0 : type;
		}
	};
}
