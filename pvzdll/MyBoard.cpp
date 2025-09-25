#include "MyBoard.hpp"

PVZ::Zombie MyBoard::AddZombieInRow(ZombieType::ZombieType type, int row, int from_wave)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EBX, from_wave)
		.push_imm32(row)
		.push_imm32(type)
		.mov_reg_imm(REG_EAX, this->BaseAddress)
		.invoke(0x40DDC0)
		.mov_mem_reg(PVZ::Memory::Variable,REG_EAX)
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
	return griditem;
}

int MyBoard::PixelToGridX(int x, int y)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, y)
		.mov_reg_imm(REG_EAX, x)
		.mov_reg_imm(REG_ECX, this->BaseAddress)
		.invoke(0x41C4C0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

int MyBoard::PixelToGridXKeepOnBoard(int x, int y)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, y)
		.mov_reg_imm(REG_ESI, x)
		.mov_reg_imm(REG_EBX, this->BaseAddress)
		.invoke(0x41C530)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

int MyBoard::PixelToGridY(int x, int y)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX, y)
		.mov_reg_imm(REG_EAX, x)
		.mov_reg_imm(REG_EDX, this->BaseAddress)
		.invoke(0x41C550)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

int MyBoard::PixelToGridYKeepOnBoard(int x, int y)
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, y)
		.mov_reg_imm(REG_EAX, x)
		.mov_reg_imm(REG_EBX, this->BaseAddress)
		.invoke(0x41C650)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

void MyBoard::GetPlantsOnLawn(int x, int y, PlantOnLawn& plants)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(y)
		.push_imm32(x)
		.mov_reg_imm(REG_EBX, (uint32_t)&plants)
		.mov_reg_imm(REG_EDX, this->BaseAddress)
		.invoke(0x40D2A0)
		.ret()
	);
}