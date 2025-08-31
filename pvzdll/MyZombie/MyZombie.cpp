#include "MyZombie.hpp"
#include "implement/index.hpp"

namespace ZombieAbility
{
	ZombiePTR pt_factory[] =
	{
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new PoleVaulter(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new DancingZombie(), new BaseZombie(),
		new BaseZombie(), new BaseZombie(), new Zomboni(),    new BaseZombie(), new BaseZombie(),
		new ClownZombie(), new BaseZombie(), new DiggerZombie(), new PogoZombie(), new BaseZombie(),
		new BungeeZombie(), new BaseZombie(), new CatapultZombie(), new BaseZombie(), new BaseZombie(),

		new BaseZombie(), new PeaZombie(),  new WallNutZombie(), new JalapenoZombie(), new BaseZombie(),
		new SquashZombie(), new TallNutZombie(), new BaseZombie()
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

void MyZombie::FlyAway(float factor)
{
	if (!this->Blowaway)
	{
		this->Blowaway = true;
		this->SetSpeed(this->Speed * factor);
	}
}

void MyZombie::AddFrost(int num)
{
	int add_frost_num = this->FrostStack + num
	this->FrostStack = add_frost_num;
	if (this->FrostStack > 30)
	{
		int damage = (this->FrostStack - 30) * 0.01f * (this->BodyHealth + this->HelmHealth + this->ShieldHealth);//溢出层数*1%*当前生命值转化为伤害
		this->Hit(damage, PVZ::DAMAGEF_NOFLASH);
		this->FrostStack = 30;
	}
	this->UpdateAnimSpeed();
	if (!this->ColorFlag)
		this->ColorFlag = 2;
}

void MyZombie::AddPoison(int num)
{
	this->PoisonStack += num;
	if (!this->ColorFlag)
		this->ColorFlag = 1;
}

void MyZombie::AddFlame(int num)
{
	this->FlameStack += num;
	//燃烬效果
	if (this->ShieldType == ShieldType::ZombieAccessoriesType2None && this->FlameStack > this->BodyHealth + this->HelmHealth)
	{
		if (this->BodyHealth >= 1800)
			this->BodyHealth = 1799;
		this->Blast();
	}
	if (!this->ColorFlag)
		this->ColorFlag = 3;
}