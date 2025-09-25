#include "pch.h"

bool CLOWN_ZOMBIE_POP_FLAG = false;

static const ZombieType::ZombieType hypno_pool[] =
{
	ZombieType::Zombie,				ZombieType::FlagZombie,			ZombieType::ConeheadZombie,									ZombieType::BucketheadZombie,
	ZombieType::NewspaperZombie,	ZombieType::ScreenDoorZombie,	ZombieType::FootballZombie, ZombieType::DancingZombie,		ZombieType::BackupDancer,
	ZombieType::DuckyTubeZombie,									ZombieType::Zomboin,		ZombieType::ZombieBobsledTeam,
	ZombieType::JackintheboxZombie,	ZombieType::BalloonZombie,		ZombieType::DiggerZombie,									ZombieType::ZombieYeti,
									ZombieType::LadderZombie,									ZombieType::Gargantuar,			ZombieType::Imp,
									ZombieType::PeashooterZombie,	ZombieType::WallnutZombie,	ZombieType::JalapenoZombie,		ZombieType::GatlingPeaZombie,
									ZombieType::TallnutZombie,		ZombieType::Gigagargantuar,
};

void onRandomZombieDropHelm(MyZombie zombie)
{
	if (zombie.Type != ZombieType::ConeheadZombie)
		return;

	zombie.Remove();

	ZombieType::ZombieType type = ZombieType::None;
	int elite_type = -1;
	if (zombie.Hypnotized)
		type = ::hypno_pool[Creator::Rand(sizeof(hypno_pool) / sizeof(ZombieType::ZombieType))];
	else
	{
		elite_type = Creator::Rand(2);
		type = static_cast<ZombieType::ZombieType>(Creator::Rand(33));
		if (type == ZombieType::DrZomboss)type = ZombieType::PeashooterZombie;//僵王换豌豆
		if (type == ZombieType::BungeeZombie)type = ZombieType::WallnutZombie;//蹦极换坚果

		switch (type)
		{
		case ZombieType::Zombie:
		case ZombieType::NewspaperZombie:
		case ZombieType::JackintheboxZombie:
		case ZombieType::PogoZombie:
		case ZombieType::CatapultZombie:
		case ZombieType::SquashZombie:
		case ZombieType::PeashooterZombie:
		case ZombieType::WallnutZombie:
		case ZombieType::TallnutZombie:
		case ZombieType::Gigagargantuar:
			elite_type = -1;
			break;
		case ZombieType::FootballZombie:
			if (elite_type > 0)
				elite_type = 2;
			break;
		case ZombieType::FlagZombie:
		case ZombieType::ScreenDoorZombie:
			elite_type = Creator::Rand(3);
			break;
		}

		elite_type = elite_type > 0 ? elite_type + WAVE_ELITE_MASK : -1;
	}

	MyBoard board = zombie.GetBoard();
	MyZombie child_zombie{ board.AddZombieInRow(type, zombie.Row, elite_type) };
	if (zombie.Hypnotized)
		child_zombie.Hypnotized = true;
	PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, zombie.Layer + 100, EffectType::IMITATER_TRANSFORMING);
	child_zombie.X = zombie.X;

	if (true)
	{
		int HpPoint = zombie.GenHpPoint();
		if (CLOWN_ZOMBIE_POP_FLAG)
			HpPoint = 5;
		child_zombie.HpPoint = HpPoint;

		child_zombie.BodyHealth = child_zombie.BodyHealth * HpPoint / 5;
		child_zombie.BodyMaxHealth = child_zombie.BodyMaxHealth * HpPoint / 5;
		child_zombie.HelmHealth = child_zombie.HelmHealth * HpPoint / 5;
		child_zombie.HelmMaxHealth = child_zombie.HelmMaxHealth * HpPoint / 5;
		child_zombie.ShieldHealth = child_zombie.ShieldHealth * HpPoint / 5;
		child_zombie.ShieldMaxHealth = child_zombie.ShieldMaxHealth * HpPoint / 5;

		if (HpPoint == 5)
			child_zombie.GoldMark = 1;
		if (HpPoint == 4)
			child_zombie.GoldMark = 2;
		///651185跳到了651373和6512d5,不知道是干啥的一段代码，没搬
	}

	//僵尸的各种属性继承
	child_zombie.FlameStack = zombie.FlameStack;
}

void InitRandomZombieEvents()
{
	PVZEvent::ZombieDropHelmByDamageEvent((int)onRandomZombieDropHelm);
}