#include "MyBoard.hpp"

void MyBoard::BurnRow(int row)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(row)
		.mov_reg_imm(REG_EDI, this->GetBaseAddress())
		.invoke(0x4664B0)
		.ret()
	);
}
