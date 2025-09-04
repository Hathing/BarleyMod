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

PVZ::Zombie MyBoard::AddZombieInRow(ZombieType::ZombieType type, int row, int from_wave)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EBX, from_wave)
		.push_imm32(row)
		.push_imm32(type)
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.invoke(0x40DDC0)
		.ret()
	);
}

PVZ::Griditem MyBoard::AddGriditem(GriditemType::GriditemType type, int row, int column)
{
	auto griditem = Creator::CreateGriditem();
	griditem.Type = type;
	griditem.Layer = 200001 + 10000 * row;
	griditem.Row = row;
	griditem.Column = column;
}
