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

	int type = 0, elite_type = -1;
	if (zombie.Hypnotized)
		type = ::hypno_pool[Creator::Rand(sizeof(hypno_pool) / sizeof(ZombieType::ZombieType))];
	else
	{
		elite_type = Creator::Rand(2);
		type = Creator::Rand(33);
		if (type == 25)type = 26;//僵王换豌豆
		if (type == 20)type = 27;//蹦极换坚果

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
	MyZombie child_zombie{ board.AddZombieInRow(static_cast<ZombieType::ZombieType>(type), zombie.Row, elite_type) };
	PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, zombie.Layer + 100, EffectType::IMITATER_TRANSFORMING);
	child_zombie.X = zombie.X;

	if (true)
	{
		int HpPoint = 1 + Creator::Rand(5);
		if (CLOWN_ZOMBIE_POP_FLAG)
			HpPoint = 5;
		child_zombie.HpPoint = HpPoint;
		float health_ratio = (HpPoint) / 5.0f;

		child_zombie.BodyHealth *= health_ratio;
		child_zombie.BodyMaxHealth *= health_ratio;
		child_zombie.HelmHealth *= health_ratio;
		child_zombie.HelmMaxHealth *= health_ratio;
		child_zombie.ShieldHealth *= health_ratio;
		child_zombie.ShieldMaxHealth *= health_ratio;
		///651185跳到了651373和6512d5,不知道是干啥的一段代码，没搬
	}
	return;
}

void InitRandomZombieEvents()
{
	PVZEvent::ZombieDropHelmByDamageEvent((int)onRandomZombieDropHelm);
}