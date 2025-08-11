#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class Zomboni : public BaseZombie
	{
		void onKilled(MyZombie zombie)
		{
			PVZ::Memory::Execute(AsmBuilder()
				.push_imm32(zombie.Row)
				.push(ZombieType::ZombieBobsledTeam)
				.mov_reg_imm(REG_EAX, zombie.GetBoard().GetBaseAddress())
				.invoke(0x40DDC0)
				.ret()
			);
		}
	};
}
