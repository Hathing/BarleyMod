#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class Gargantaur : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetAnimation();
			model.AssignRenderGroupToPrefix(-1, "anim_bucket");
			model.AssignRenderGroupToPrefix(-1, "anim_screendoor");

			MyBoard board = zombie.GetBoard();
			if (board.isValid())
			{
				int tmp = max(3500, 2000 + board.MatchTimer / 80);
				zombie.HelmHealth = tmp;
				zombie.HelmMaxHealth = tmp;
			}
		}
		bool OverrideDrawPos(MyZombie zombie, PVZ::ZombieDrawPosition* draw_pos)
		{
			if (zombie.Hypnotized)
				draw_pos->ImageOffsetX += 30.0f;
			return true;
		}
	};
	class GigaGargantaur : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			MyBoard board = zombie.GetBoard();
			if (board.isValid())
			{
				int tmp = max(7000, 4000 + board.MatchTimer / 40);
				zombie.HelmHealth = tmp;
				zombie.HelmMaxHealth = tmp;
			}
		}
		bool OverrideDrawPos(MyZombie zombie, PVZ::ZombieDrawPosition* draw_pos)
		{
			if (zombie.Hypnotized)
				draw_pos->ImageOffsetX += 30.0f;
			return true;
		}
	};
}
