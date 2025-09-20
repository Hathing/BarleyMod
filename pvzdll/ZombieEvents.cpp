#include "pch.h"
#include "MyPlant/PlantAbility.hpp"
#include "MyZombie/ZombieAbility.hpp"
#include "MyEvents.hpp"

bool CLOWN_ZOMBIE_POP_FLAG = false;

void onZombieDropLoot(MyZombie zombie)
{
	zombie.DroppedLoot = 1;

	if (zombie.Type == ZombieType::DrZomboss)
		return;
	
	if (zombie.PoisonStack >= 10)
	{
		int spread_stack = zombie.PoisonStack >> 2;
		auto zombies = zombie.GetBoard().GetAllZombies<MyZombie>();
		for (auto myzombie : zombies)
			if (myzombie.Row == zombie.Row && myzombie.X - 80 >= zombie.X && myzombie.X + 80 <= zombie.X
					&& myzombie.Type != ZombieType::DrZomboss)
				myzombie.PoisonStack += spread_stack;
	}

	if (!zombie.Hypnotized)
	{
		int bounty_xp = zombie.GetBountyXP();
		auto attacker = MyPlant::GetByID(zombie.LastDamageSourceID).GetOwner();

		if (attacker.isValid() && attacker.IsXPRecipient())
		{
			PlantAbility::GetAbility(attacker.Type)->onKill(attacker, zombie);
			attacker.AddExperience(bounty_xp * 4 / 5, true);
			bounty_xp -= (bounty_xp * 4 / 5);
		}

		auto plants = zombie.GetBoard().GetAllPlants<MyPlant>();
		int plant_cnt = 0;
		for (auto plant : plants)
			if (plant.Row == zombie.Row && plant.IsXPRecipient())
				plant_cnt++;
		if (plant_cnt <= 0) return;
		bounty_xp /= plant_cnt;
		for (auto plant : plants)
			if (plant.Row == zombie.Row && plant.IsXPRecipient())
				plant.AddExperience(bounty_xp);
	}
}

void onZombieInitAfter(MyZombie zombie)
{
	zombie.IsWalkingBackwards = 0;
	zombie.IsWeak = false;
	
	zombie.ColorFlag = 0;
	zombie.HpPoint = 0;
	if (zombie.Type != ZombieType::BalloonZombie)
		zombie.SourceID = 0;
	zombie.SourceLevel = 0;
	zombie.GhostFlameMark = 0;

	zombie.FrostStack = 0;
	zombie.PoisonStack = 0;
	zombie.FlameStack = 0;

	zombie.LastDamageSourceID = 0;
	ZombieAbility::GetAbility(zombie.Type)->onCreated(zombie);
}

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
	PVZ::CreateParticleSystem(zombie.X + 40.0f,zombie.Y + 65.0f,zombie.Layer+100,EffectType::IMITATER_TRANSFORMING);
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

int onPlantTakeDamage(MyPlant plant, PVZ::BaseClass source, GameObjectType::GameObjectType source_type, int damage)
{
	return damage;
}

bool onZombieSquishPlant(MyZombie zombie, int row, int column, int attack_type, MyPlant plant)
{
	auto zombietype = zombie.Type;
	auto planttype = plant.Type;
	if (zombietype == ZombieType::Zomboin || zombietype == ZombieType::CatapultZombie)
	{
		if (planttype == SeedType::Wallnut || planttype == SeedType::Tallnut)
		{
			zombie.X += 50.0f;//击退距离
			//这里应该将 僵尸伤害植物 和 植物伤害僵尸 和 子弹伤害僵尸 分别封装成一个函数，避免某些原本该触发的事件未触发
			zombie.Hit(200, PVZ::DAMAGEF_NONE);
			int damage = onPlantTakeDamage(plant, zombie, GameObjectType::OBJECT_TYPE_NONE, 500);//这里object type没有僵尸？
			plant.Hp -= damage;//植物碾压受伤
			return false;
		}
	}
	return true;
}

void onLoadPlainZombieReanimBefore(MyZombie zombie)
{
	//隐藏鬼火
	zombie.ReanimShowPrefix("guih", -1);
	zombie.ReanimShowPrefix("wenhao", -1);
}

float onZombieUpdateWalkingSpeed(MyZombie zombie, float velocity)
{
	if (zombie.IsTangleKelpTarget())
		return 0.0f;

	if (zombie.X > zombie.GetBoard().GetIcetrace().GetX(zombie.Row) - 40)
		return velocity * 2.0f;
	return velocity;
}

int onZombieIsWalkingBackwards(MyZombie zombie)
{
	if (zombie.IsWalkingBackwards == 2)
		return 1;
	if (zombie.IsWalkingBackwards == 1)
	{
		if (zombie.X > 725.0)
		{
			zombie.IsWalkingBackwards = 0;
			return 0;
		}
		return 1;
	}
	return -1;
}
float onZombieApplyAnimSpeed(MyZombie zombie, PVZ::Animation anim, float rate)
{
	if (zombie.FrostStack && zombie.FrostStack !=0xFF)//在InitAfter之前会调用一次AnimSpeed，此时僵尸的FrostStack还没有初始化，初始值为0xFF
	{
		rate *= max(1.0f - zombie.FrostStack * 0.02f,0.4f);
	}
	return rate;
}
void onZombieUpdatePlaying(MyZombie zombie)
{
	//僵尸的总更新，无视黄油和冻结
	if (!zombie.Hypnotized)
	{
		return;
	}
	return;
}

bool onZombieUpdateColor(MyZombie zombie,PVZ::Animation anim,int red,int green,int blue,int alpha)
{
	if (zombie.HpPoint == 5)
	{
		PVZ::Color color{ 248,255,0,alpha };
		anim.SetColor(color);
		anim.SetAdditiveColor(color);
		anim.DrawAdditiveColor = true;
		return false;
	}
	auto colorflag = zombie.ColorFlag;
	if (colorflag)
	{
		float ratio = 0.8f;
		switch (colorflag)
		{
		case 1:
			ratio -= 0.015f * min(zombie.PoisonStack, 40);
			red *= ratio;
			blue *= ratio;
			break;
		case 2:
			ratio -= 0.02f * zombie.FrostStack;
			red *= ratio;
			green *= ratio;
			break;
		case 3:
			ratio -= 0.001f * min(zombie.FlameStack, 600);
			green *= ratio;
			blue *= ratio;
			break;
		default:
			break;
		}
		PVZ::Color color{ red,green,blue,alpha };
		anim.SetColor(color);
		anim.SetAdditiveColor(color);
		//anim.DrawAdditiveColor = false;
		return false;
	}
	return true;
}

bool onZombieUpdateAction(MyZombie zombie)
{
	//这里是所有僵尸在未定身时必经的更新
	MyBoard board = zombie.GetBoard();
	//小丑僵尸爆炸
	if (zombie.Type == ZombieType::JackintheboxZombie && zombie.State == ZombieState::JACKBOX_WALKING && ((!zombie.Hypnotized && zombie.IsWalkingBackwards == 0) || (zombie.Hypnotized && zombie.X > 725.0f)))
	{
		zombie.AttributeCountdown = 0;
	}
	if (zombie.Type == ZombieType::DiggerZombie && zombie.State==ZombieState::DIGGER_DIG)
	{
		int col = board.PixelToGridXKeepOnBoard(zombie.X + 80.0f, zombie.Y);
		if (col < zombie.DiggerLastDigRootColumn)
		{
			zombie.DiggerLastDigRootColumn = col;
			PlantOnLawn dug_plants{};
			board.GetPlantsOnLawn(col, zombie.Row, dug_plants);
			MyPlant dugplant = dug_plants.MainPlant;
			//不知道为什么，当矿工没挖到植物时，dugplant会莫名指向场上的其他植物，原因未知，好像是40D2A0自身的特性？
			//下面暂时加了一个特殊判定
			if (dugplant.isValid() && dugplant.Row == zombie.Row && dugplant.Column == col)
			{
				dugplant.HpDisplayCounter = 100;
				if (dugplant.Type == SeedType::Spikerock)
					zombie.DiggerLoseAxe();
			}
		}
	}
	return zombie.ZombieHeight != 9 || (zombie.Type != ZombieType::CatapultZombie && zombie.Type != ZombieType::Zomboin);
}

bool onClownZombiePop(MyZombie zombie, int x, int y)
{
	CLOWN_ZOMBIE_POP_FLAG = true;
	auto zombies = zombie.GetBoard().GetAllZombies<MyZombie>();
	for (auto myzombie : zombies)
		if (myzombie.Row == zombie.Row && myzombie.X + 115 >= zombie.X && myzombie.X - 115 <= zombie.X && myzombie.Type == ZombieType::ConeheadZombie)
			myzombie.Hit(myzombie.HelmMaxHealth + 1, PVZ::DAMAGEF_NOLEAVEBODY);
		
	CLOWN_ZOMBIE_POP_FLAG = false;
	return false;
}

bool onHypnotizedClownZombiePop(MyZombie zombie, int x, int y)
{
	CLOWN_ZOMBIE_POP_FLAG = true;
	auto zombies = zombie.GetBoard().GetAllZombies<MyZombie>();
	for (auto myzombie : zombies)
		if (myzombie.X + 115 >= zombie.X && myzombie.X - 115 <= zombie.X)
		{
			if (myzombie.Row == zombie.Row)
				myzombie.Blast();
			else if ((myzombie.Row == zombie.Row + 1 || myzombie.Row == zombie.Row - 1) && myzombie.Type == ZombieType::ConeheadZombie)
				myzombie.Hit(myzombie.HelmMaxHealth + 1, PVZ::DAMAGEF_NOLEAVEBODY);
		}
	CLOWN_ZOMBIE_POP_FLAG = false;
	return false;
}

bool onZombieEatSound(MyZombie zombie, MyPlant plant)
{
	if (zombie.Hypnotized)
		return false;
	return true;
}

bool onZombieWalkIntoWater(MyZombie zombie)
{
	if (!zombie.InWater && zombie.IsTangleKelpTarget())
		return false;
	return true;
}

bool onZombieWalkOutOfWater(MyZombie zombie)
{
	if (zombie.IsTangleKelpTarget() || !zombie.NotDying)
		return false;
	return true;
}

int onZombieCanTargetPlant(MyZombie zombie, MyPlant plant, int AttackType)
{
	if (zombie.IsTangleKelpTarget())
	{
		return 0;
	}
	if (zombie.State == ZombieState::DIGGER_WALK_RIGHT && zombie.X < 130)
		return 0;
	return -1;
}

int onZombieTakeDmg(MyZombie zombie,PVZ::DamageFlags dmg_flags,int dmg)
{
	//读报无敌
	if (zombie.State == ZombieState::NEWSPAPER_DESTORYED)
	{
		//防止毒之类的伤害疯狂刷读报怒气
		if (dmg_flags != PVZ::DAMAGEF_NOFLASH && zombie.NewspaperAngerStack < 20)
		{
			zombie.NewspaperAngerStack += 1;
		}
		return 0;
	}
	return dmg;
}

bool onZombiePickRandomSpeed(MyZombie zombie)
{
	if (zombie.State == ZombieState::NEWSPAPER_RUNNING)
	{
		zombie.SetSpeed(0.9f * (zombie.NewspaperAngerStack / 5.0f + 1.0f));//原版读报暴走移速区间为0.89-0.91
		return false;
	}

	switch (zombie.State)
	{
	case ZombieState::NEWSPAPER_RUNNING:
		zombie.SetSpeed(0.9f * (zombie.NewspaperAngerStack / 5.0f + 1.0f));//原版读报暴走移速区间为0.89-0.91
		return false;
	case ZombieState::DIGGER_WALK_RIGHT:
		zombie.SetSpeed(0.24f);
		return false;
	default:
		break;
	}

	return true;
}

bool onPoleVaulterHalfJump(MyZombie zombie, MyPlant plant)
{
	return zombie.FromWave < WAVE_ELITE_MASK;
}

void InitZombieEvents()
{
	PlantTakeDamageEvent((int)onPlantTakeDamage);
	ZombieDropLootEvent((int)onZombieDropLoot);
	ZombieInitAfterEvent((int)onZombieInitAfter);
	PVZEvent::ZombieDropHelmByDamageEvent((int)onRandomZombieDropHelm);
	PVZEvent::ZombieSquishPlantEvent((int)onZombieSquishPlant);
	PVZEvent::LoadPlainZombieReanimBeforeEvent((int)onLoadPlainZombieReanimBefore);
	PVZEvent::ZombieUpdateWalkingSpeedEvent((int)onZombieUpdateWalkingSpeed);
	PVZEvent::ZombieIsWalkingBackwardsEvent((int)onZombieIsWalkingBackwards);
	PVZEvent::ZombieApplyAnimSpeedEvent((int)onZombieApplyAnimSpeed);
	ZombieUpdatePlayingEvent((int)onZombieUpdatePlaying);
	PVZEvent::ZombieUpdateColorEvent((int)onZombieUpdateColor);
	PVZEvent::ZombieUpdateActionEXEvent((int)onZombieUpdateAction);
	PVZEvent::ClownZombiePopEvent((int)onClownZombiePop);
	PVZEvent::HypnotizedClownZombiePopEvent((int)onHypnotizedClownZombiePop);
	ZombieEatSoundEvent((int)onZombieEatSound);
	PVZEvent::ZombieWalkIntoWaterEvent((int)onZombieWalkIntoWater);
	PVZEvent::ZombieWalkOutOfWaterEvent((int)onZombieWalkOutOfWater);
	ZombieTargetPlantEvent((int)onZombieCanTargetPlant);
	ZombieTakeDmgEvent((int)onZombieTakeDmg);
	PVZEvent::ZombiePickRandomSpeedEvent((int)onZombiePickRandomSpeed);

	PVZEvent::PoleVaulter::HalfJumpEvent((int)onPoleVaulterHalfJump);

	//修改冰道持续时间
	PVZ::Memory::WriteMemory<int>(0x52A8B6, 1000);
	//矿工正常出土时右行而非左行
	PVZ::Memory::WriteMemory<int>(0x52874E, 0x00000025);
	//矿工从底线出土
	PVZ::Memory::WriteMemory<int>(0x528334, 0x0000041F);
	//覆盖原盲盒开盒
	static constexpr byte asm_revert_1[] = {MOV_PTR_EUX_ADD(REG_EBX, 0x0C4, 0)};
	PVZ::Memory::WriteArray<const byte>(0x530FC4, STRING(asm_revert_1));
	//覆盖原金银特效
	static constexpr byte asm_revert_2[] = { 0x83,0xBE,0xAC,0x00,0x00,0x00,0x00 };
	PVZ::Memory::WriteArray<const byte>(0x52D309, STRING(asm_revert_2));
}