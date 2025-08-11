#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class PoleVaulter : public BaseZombie
	{
		void onKilled(MyZombie zombie)
		{
			if (zombie.EliteType == 1 && zombie.State == ZombieState::POLE_VALUTING_RUNNING)
			{
				PVZ::Memory::WriteMemoryUnsafe<int>(0x701200, 0);
				Creator::CreateZombie(ZombieType::PoleVaultingZombie, zombie.Row, 10).X = zombie.X + 20;
			}
		}
	};
}
