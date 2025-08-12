#include "MyZombie.hpp"
#include "implement/index.hpp"

namespace ZombieAbility
{
	ZombiePTR pt_factory[] =
	{
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new PoleVaulter(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new Zomboni(),    new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new DiggerZombie(), new PogoZombie(), new BaseZombie(),
		new BungeeZombie(), new BaseZombie(), new CatapultZombie(), new BaseZombie(), new BaseZombie(),

		new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie()
	};
}

ZombieAbility::ZombiePTR ZombieAbility::GetAbility(ZombieType::ZombieType type)
{
	return ZombieAbility::pt_factory[type];
}

int MyZombie::GetBountyXP()
{
	if (this->Hypnotized || this->Unknown == 9 || this->Type == ZombieType::ConeheadZombie)
		return 0;
	else
		return this->BodyMaxHealth + this->HelmMaxHealth + this->ShieldMaxHealth;
}

void MyZombie::AttachShield()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x533000)
		.ret()
	);
}
