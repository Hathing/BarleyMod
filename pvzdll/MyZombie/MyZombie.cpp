#include "MyZombie.hpp"
#include "implement/index.hpp"

namespace ZombieAbility
{
	ZombiePTR pt_factory[] =
	{
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),

		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie()
	};
}

ZombieAbility::ZombiePTR ZombieAbility::GetAbility(ZombieType::ZombieType type)
{
	return ZombieAbility::pt_factory[type];
}
/*
void MyZombie::onCreated()
{
	ZombiePrototype::pt_factory[this->Type]->onCreated(*this);
}
*/

void MyZombie::AttachShield()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x533000)
		.ret()
	);
}
