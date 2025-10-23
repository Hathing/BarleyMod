#include "MyZombie.hpp"
#include "implement/index.hpp"

namespace ZombieAbility
{
	ZombiePTR pt_factory[] =
	{
		new Undying(),			new FlagZombie(),			new RandomZombie(),		new PoleVaulter(),			new BucketHeadZombie(),
		new NewspaperZombie(),	new ScreenDoorZombie(),		new FootballZombie(),	new DancingZombie(),		new BaseZombie(),
		new BaseZombie(),		new SnorkedZombie(),		new Zomboni(),			new ZombieBobsledTeam(),	new DolphinRiderZombie(),
		new ClownZombie(),		new BalloonZombie(),		new DiggerZombie(),		new PogoZombie(),			new ZombieYeti(),
		new BungeeZombie(),		new BaseZombie(),			new CatapultZombie(),	new Gargantuar(),			new Imp(),

		new BaseZombie(),		new PeaZombie(),			new WallNutZombie(),	new JalapenoZombie(),		new BaseZombie(),
		new SquashZombie(),		new TallNutZombie(),		new GigaGargantuar()
	};
}

ZombieAbility::ZombiePTR ZombieAbility::GetAbility(ZombieType::ZombieType type)
{
	return ZombieAbility::pt_factory[type];
}

int MyZombie::GetBountyXP()
{
	if (this->Hypnotized || this->Type == ZombieType::ConeheadZombie)
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

void MyZombie::AttachSeaweed(const char* trackName, float x, float y, float scale)
{
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, trackName, std::strlen(trackName) + 1);
	auto particles = PVZ::CreateParticleSystem(0.0f,0.0f,0,EffectType::SEAWEED_IN_CORAL_THREE);
	particles.OverrideScale(scale);
	auto anim = this->GetAnimation();
	PVZ::Memory::Execute(AsmBuilder()
		.push_float(y).push_float(x).push_imm32(particles.GetBaseAddress())
		.mov_reg_imm(REG_EAX, PVZ::Memory::Variable + 100)
		.mov_reg_imm(REG_ECX, anim.GetBaseAddress())
		.invoke(0x473070)
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

void MyZombie::ShiftColorFlag()
{
	switch (this->ColorFlag)
	{
	case 1:
	{
		if (this->FrostStack)
			this->ColorFlag = 2;
		else if (this->FlameStack > 0)
			this->ColorFlag = 3;
		else if (this->PoisonStack <= 0)
			this->ColorFlag = 0;
		break;
	}
	case 2:
	{
		if (this->FlameStack > 0)
			this->ColorFlag = 3;
		else if (this->PoisonStack > 0)
			this->ColorFlag = 1;
		else if (this->FrostStack == 0)
			this->ColorFlag = 0;
		break;
	}
	case 3:
	{
		if (this->PoisonStack > 0)
			this->ColorFlag = 1;
		else if (this->FrostStack)
			this->ColorFlag = 2;
		else if (this->FlameStack <= 0)
			this->ColorFlag = 0;
		break;
	}
	default:
	{
		if (this->PoisonStack > 0)
			this->ColorFlag = 1;
		else if (this->FrostStack)
			this->ColorFlag = 2;
		else if (this->FlameStack > 0)
			this->ColorFlag = 3;
		else
			this->ColorFlag = 0;
		break;
	}
	}
}

MyZombie MyZombie::FindZombieTarget()
{
	int targetaddr = PVZ::Memory::Execute(AsmBuilder().push_imm32(this->GetBaseAddress()).invoke(0x52E840).mov_mem_reg(Memory::Variable, REG_EAX).ret());
	return MyZombie{ targetaddr };
}

PVZ::Plant MyZombie::FindPlantTarget(int attacktype)
{
	int targetaddr = PVZ::Memory::Execute(AsmBuilder().push_imm32(attacktype).push_imm32(this->GetBaseAddress()).invoke(0x52E780).mov_mem_reg(Memory::Variable, REG_EAX).ret());
	return PVZ::Plant{ targetaddr };
}

void MyZombie::StopEating()
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EDI,this->GetBaseAddress()).invoke(0x52F440).ret());
}

void MyZombie::StartWalkAnim(int blendtime)
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_ESI, blendtime).mov_reg_imm(REG_EAX, this->GetBaseAddress()).invoke(0x52F2E0).ret());
}

void MyZombie::PlayAnim(const char* trackname, float animrate, int blendtime, int looptype)
{
	PVZ::Memory::WriteArray<const char>(PVZ::Memory::Variable + 100, trackname, std::strlen(trackname) + 1);
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(*(unsigned int*)&animrate)
		.push_imm32(blendtime)
		.push_imm32(looptype)
		.push_imm32(PVZ::Memory::Variable + 100)
		.mov_reg_imm(REG_EDI, this->GetBaseAddress())
		.invoke(0x528B00).ret());
}

void MyZombie::PickRandomSpeed()
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EAX, this->GetBaseAddress()).invoke(0x524A70).ret());
}

void MyZombie::RiseFromGrave(int row, int column)
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EBX, this->GetBaseAddress()).mov_reg_imm(REG_EAX,column).push_imm32(row).invoke(0x531C90).ret());
}

void MyZombie::AddFrost(int num)
{
	int add_frost_num = this->FrostStack + num;
	if (add_frost_num > 30)
	{
		int damage = (add_frost_num - 30) * 0.01f * (this->BodyHealth + this->HelmHealth + this->ShieldHealth);//溢出层数*1%*当前生命值转化为伤害
		this->Hit(damage, PVZ::DAMAGEF_NOFLASH);
		add_frost_num = 30;
	}
	this->FrostStack = add_frost_num;
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

void MyZombie::Launch(float xspeed, float yspeed, float startheight)
{
	if (this->IsLaunched == 0 && this->ZombieHeight == 0)
	{
		if (this->Eating)
			this->StopEating();
		this->IsLaunched = 1;
		this->ZombieHeight = 7;
		this->Height = startheight;
		this->Speed = xspeed;
		this->FallSpeed = yspeed;
	}
}
/// @brief 僵尸被击退
void MyZombie::KnockBack(float xspeed)
{
	if (this->IsLaunched == 0 && this->ZombieHeight == 0)
	{
		if (this->Eating)
			this->StopEating();
		this->IsLaunched = 1;
		this->Speed = xspeed;
	}
}

bool MyZombie::IsNotWalking()
{
	int result = PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x52A610)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
	return result & 0x000000FF;
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

void MyZombie::DiggerLoseAxe()
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, this->GetBaseAddress())
		.invoke(0x528240)
		.ret()
	);
}

int MyZombie::FindCatapultTarget()
{
	return PVZ::Memory::Execute(AsmBuilder()
		.push_imm32( this->GetBaseAddress())
		.invoke(0x525890)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

void MyZombie::ZombieCatapultFire(int targetaddr)
{
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_EAX, targetaddr)
		.mov_reg_imm(REG_ECX, this->GetBaseAddress())
		.invoke(0x525730)
		.ret()
	);
}

int MyZombie::GetBobsledPosition()
{
	return PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX,this->GetBaseAddress())
		.invoke(0x5346A0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
}

static constexpr int HpPointTable[5][5] =
{
	{1, 2, 3, 4, 4},
	{1, 2, 3, 4, 5},
	{2, 2, 3, 4, 5},
	{2, 3, 3, 4, 5},
	{3, 3, 3, 4, 5}
};
static constexpr int HpPointTable_9[7] = {3, 3, 3, 4, 4, 5, 5};

int MyZombie::GenHpPoint()
{
	if (this->FromWave >= WAVE_ELITE_MASK)
		return 5;

	MyBoard board = this->GetBoard();
	int duration_2min = board.MatchTimer / 12000;

	switch (duration_2min)
	{
	case 0:
	case 1:
		return 1 + Creator::Rand(3);
	case 2:
	{
		int tmp = 1 + Creator::Rand(4);
		return tmp == 4 ? 3 : tmp;
	}
	case 3:
		return 1 + Creator::Rand(4);
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		return HpPointTable[duration_2min - 4][Creator::Rand(5)];
	case 9:
		return HpPointTable_9[Creator::Rand(7)];
	}

	// default
	return 2 + Creator::Rand(3);
}
