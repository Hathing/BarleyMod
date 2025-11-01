#include "MyClasses.hpp"

void PVZ::ApplyPZDamage(MyPlant plant, MyZombie zombie, int damage, PVZ::DamageFlags flags)
{
	PZDamageEvent event{ zombie, plant, flags, damage, PVZEvent::PLANTDAMAGETYPE_CUSTOM};
	onPlantDamageZombie(&event);
	event.zombie.Hit(event.damage, event.flags);
}


bool PVZ::ApplyZPDamage(MyZombie zombie, MyPlant plant, int damage)
{
	int mydmg = onPlantTakeDamage(plant, zombie, GameObjectType::OBJECT_TYPE_NONE, damage);
	plant.Hp -= mydmg;
	//这里可以增加死亡判断，自定义死亡类型，否则植物会在原版总更新中因HP < 0 而死。

	return plant.Hp >= 0;
}

void DestroyPString(Draw::PString string)
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_ECX, string).invoke(0x404420).ret());
}

MyPlant NoiselessCreatePlant(SeedType::SeedType type, int row, int col, SeedType::SeedType ImitaterType)
{
	auto board = PVZ::GetBoard();
	int addr = PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(ImitaterType).push_imm32(type).push_imm32(row).push_imm32(col)
		.mov_reg_imm(REG_EAX,board.GetBaseAddress())
		.invoke(0x40CE20).mov_mem_reg(PVZ::Memory::Variable,REG_EAX).ret()
	);
	return MyPlant{ addr };
}