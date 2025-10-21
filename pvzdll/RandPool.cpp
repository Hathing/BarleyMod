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
	if (zombie.Type == ZombieType::FootballZombie)
	{
		//橄榄球受伤掉头盔时，冲刺时间清空
		zombie.FootballZombieChargeTime = 0;
		return;
	}
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
		//case ZombieType::FootballZombie:
		//	if (elite_type > 0)
		//		elite_type = 2;
		//	break;
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
	//雪橇小队修正X坐标
	if (child_zombie.GetBobsledPosition() == 0)
	{
		for (int i = 1; i <= 3; i++)
		{
			auto zombie_ = PVZ::GetByID<MyZombie>(child_zombie.GetRelatedZombieID(i));
			zombie_.X = child_zombie.X + 50.0f * i;
		}
	}
	//小鬼向前飞跃
	if (child_zombie.Type == ZombieType::Imp)
	{
		if (child_zombie.X > 280.0f)
		{
			child_zombie.Height = 5.0f;
			child_zombie.State = ZombieState::IMP_FLYING;

			float flyingtime = 0.8f + Creator::RandFloat(0.4f);//单位为秒
			flyingtime *= 100.0f;

			child_zombie.Speed = (child_zombie.X - 200.0f) / flyingtime;
			child_zombie.Speed += (Creator::Rand(11) - 5) * 0.2f;
			child_zombie.FallSpeed = 0.025f * flyingtime;
			child_zombie.PlayAnim("anim_thrown", 18.0f, 0, 3);
		}
	}

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