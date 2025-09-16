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
	
	zombie.ColorFlag = 0;
	zombie.HpPoint = 0;

	zombie.FrostStack = 0;
	zombie.PoisonStack = 0;
	zombie.FlameStack = 0;

	zombie.LastDamageSourceID = 0;
	ZombieAbility::GetAbility(zombie.Type)->onCreated(zombie);
}

void onRandomZombieDropHelm(MyZombie zombie)
{
	if (zombie.Type != ZombieType::ConeheadZombie)
		return;

	zombie.Remove();
	int type = Creator::Rand(33);
	if (type == 25)type = 26;//僵王换豌豆
	if (type == 20)type = 27;//蹦极换坚果
	MyZombie child_zombie{ Creator::CreateZombie(static_cast<ZombieType::ZombieType>(type), zombie.Row, 0x0f) };
	PVZ::CreateParticleSystem(zombie.X + 40.0f,zombie.Y + 65.0f,zombie.Layer+100,EffectType::IMITATER_TRANSFORMING);
	child_zombie.X = zombie.X;

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
void onZombieUpdateAction(MyZombie zombie)
{
	//这里是所有僵尸在未定身时必经的更新
	//小丑僵尸爆炸
	if (zombie.Type == ZombieType::JackintheboxZombie && zombie.State == ZombieState::JACKBOX_WALKING && ((!zombie.Hypnotized && zombie.IsWalkingBackwards == 0) || (zombie.Hypnotized && zombie.X > 725.0f)))
	{
		zombie.AttributeCountdown = 0;
	}
	return;
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
	ZombieUpdateActionEvent((int)onZombieUpdateAction);
	PVZEvent::ClownZombiePopEvent((int)onClownZombiePop);
	PVZEvent::HypnotizedClownZombiePopEvent((int)onHypnotizedClownZombiePop);
	ZombieEatSoundEvent((int)onZombieEatSound);

	//修改冰道持续时间
	PVZ::Memory::WriteMemory<int>(0x52A8B6, 1000);
	//覆盖原盲盒开盒
	static constexpr byte asm_revert_1[] = {MOV_PTR_EUX_ADD(REG_EBX, 0x0D0, 0)};
	PVZ::Memory::WriteArray<const byte>(0x530FC4, STRING(asm_revert_1));
	//覆盖原金银特效
	static constexpr byte asm_revert_2[] = { 0x83,0xBE,0xAC,0x00,0x00,0x00,0x00 };
	PVZ::Memory::WriteArray<const byte>(0x52D309, STRING(asm_revert_2));
}