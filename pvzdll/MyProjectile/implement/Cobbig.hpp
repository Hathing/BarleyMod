#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Cobbig : public BaseProjectile
	{
	public:
		int GetImage(MyProjectile proj)
		{
			return proj.SourceType == SeedType::Mangosteen ? PVZ::Memory::ReadMemory<int>(0x6FF158) : -1;
		}
		bool onImpact(MyProjectile proj, MyZombie zombie)
		{
			//爆炸
			/*
			PVZ::Memory::Execute(AsmBuilder()
				.push_imm32(PVZ::DRF_ALL)
				.push_imm32(1)//是否Burn
				.push_imm32(1)//RowRange
				.push_imm32(80)//Radius
				.push_imm32((int)proj.Y)//int y
				.push_imm32((int)proj.X)//int x
				.push_imm32(proj.Row)//Row
				.push_imm32(proj.GetBoard().GetBaseAddress())
				.invoke(0x41D8A0).ret());
			*/
			// 只炸本行僵尸
			/*
			MyBoard board = proj.GetBoard();
			auto zombies = board.GetAllZombies<MyZombie>();
			for (auto& zombie : zombies)
			{
				if (zombie.Row == proj.Row && !zombie.Hypnotized)
				{
					
				}

			}
			*/
			return true;
		}
	};
}
