#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class PoleVaulter : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			if (zombie.FromWave == WAVE_ELITE1)
			{
				zombie.BodyHealth = 700;
				zombie.BodyMaxHealth = 700;
			}
			else
			{
				zombie.BodyHealth = 800;
				zombie.BodyMaxHealth = 800;
			}
		}
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
