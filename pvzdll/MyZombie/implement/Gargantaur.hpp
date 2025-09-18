#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class Gargantaur : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			MyBoard board = zombie.GetBoard();
			if (board.isValid())
			{
				int tmp = max(3500, 2000 + board.MatchTimer / 80);
				zombie.HelmHealth = tmp;
				zombie.HelmMaxHealth = tmp;
			}
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
	};
}
