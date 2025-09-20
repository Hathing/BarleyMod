#include "MyZombie.hpp"
#include "implement/index.hpp"

namespace ZombieAbility
{
	ZombiePTR pt_factory[] =
	{
		new BaseZombie(), new BaseZombie(), new BaseZombie(), new PoleVaulter(), new BaseZombie(),
		new NewspaperZombie(), new ScreenDoorZombie(), new BaseZombie(), new DancingZombie(), new BaseZombie(),
		new BaseZombie(), new SnorkedZombie(), new Zomboni(),    new BaseZombie(), new DolphinRiderZombie(),
		new ClownZombie(), new BalloonZombie(), new DiggerZombie(), new PogoZombie(), new BaseZombie(),
		new BungeeZombie(), new BaseZombie(), new CatapultZombie(), new Gargantaur(), new BaseZombie(),

		new BaseZombie(), new PeaZombie(),  new WallNutZombie(), new JalapenoZombie(), new BaseZombie(),
		new SquashZombie(), new TallNutZombie(), new GigaGargantaur()
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

MyZombie MyZombie::FindZombieTarget()
{
	int targetaddr = PVZ::Memory::Execute(AsmBuilder().push_imm32(this->GetBaseAddress()).invoke(0x52E840).mov_mem_reg(Memory::Variable, REG_EAX).ret());
	return MyZombie{ targetaddr };
}

void MyZombie::StopEating()
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EDI,this->GetBaseAddress()).invoke(0x52F440).ret());
}

void MyZombie::AddFrost(int num)
{
	int add_frost_num = this->FrostStack + num;
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

bool MyZombie::IsTangleKelpTarget()
{
	int result = PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EDI, this->GetBaseAddress())
		.invoke(0x5324B0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
	return result & 0x000000FF ;
}