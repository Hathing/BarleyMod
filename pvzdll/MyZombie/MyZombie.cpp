#include "MyZombie.hpp"

/*
namespace ZombiePrototype
{
	PrototypePTR pt_factory[] =
	{
		new BasePrototype(), new BasePrototype(), new BasePrototype(), new BasePrototype(), new BasePrototype(),
		new NewspaperZombie(), new BasePrototype(), new BasePrototype(), new DancerZombie(), new BasePrototype(),
		new BasePrototype(), new SnorkedZombie(), new BasePrototype(), new BasePrototype(), new BasePrototype(),
		new BasePrototype(), new BasePrototype(), new DiggerZombie(), new PogoZombie(), new BasePrototype(),
		new BungeeZombie(), new CatapultZombie(), new BasePrototype(), new BasePrototype(), new BasePrototype(),
		new BasePrototype(), new PeaZombie(), new BasePrototype(), new JalapenoZombie(), new BasePrototype(),
		new SquashZombie(), new TallNutZombie(), new BasePrototype(), new BasePrototype(), new BasePrototype(),
		new BasePrototype(), new BasePrototype(), new BasePrototype(), new BasePrototype(), new BasePrototype(),
	};
}

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
