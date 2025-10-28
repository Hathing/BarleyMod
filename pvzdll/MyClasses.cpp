#include "MyClasses.hpp"

void PVZ::ApplyPZDamage(MyPlant plant, MyZombie zombie, int damage, PVZ::DamageFlags flags)
{
	PZDamageEvent event{ zombie, plant, flags, damage, PVZEvent::PLANTDAMAGETYPE_CUSTOM};
	onPlantDamageZombie(&event);
	event.zombie.Hit(event.damage, event.flags);
}


void PVZ::ApplyZPDamage(MyZombie zombie, MyPlant plant, int damage)
{
	int mydmg = onPlantTakeDamage(plant, zombie, GameObjectType::OBJECT_TYPE_NONE, damage);
	plant.Hp -= mydmg;
	//这里可以增加死亡判断，自定义死亡类型，否则植物会在原版总更新中因HP < 0 而死。
}

void DestroyPString(Draw::PString string)
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_ECX, string).invoke(0x404420).ret());
}

MyPlant NoiselessCreatePlant(SeedType::SeedType type, int row, int col, SeedType::SeedType ImitaterType)
{
	auto board = PVZ::GetBoard();
	int addr = PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ESI, board.GetBaseAddress()+0xAC).invoke(0x41DE80)
		.mov_reg_reg(REG_EDI, REG_EAX)
		.add_byte(0xC6).add_byte(0x87).add_dword(0x00000144).add_byte(0x01)//mov byte[edi+144],1
		.push_imm32(ImitaterType).push_imm32(type).push_reg(REG_EAX)
		.mov_reg_imm(REG_EAX,row).mov_reg_imm(REG_ECX,col)
		.invoke(0x45DB60).mov_mem_reg(PVZ::Memory::Variable,REG_EDI).ret()
	);
	return MyPlant{ addr };
}