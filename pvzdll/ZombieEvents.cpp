#include "pch.h"
#include "MyPlant/PlantAbility.hpp"
#include "MyZombie/ZombieAbility.hpp"
#include "MyEvents.hpp"

#define FROST_DECELERATE(zombie) max(1.0f - (zombie).FrostStack * 0.02f,0.4f)

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
	zombie.Taunted = false;
	
	zombie.ColorFlag = 0;
	zombie.HpPoint = 0;
	if (zombie.Type != ZombieType::BalloonZombie)
		zombie.SourceID = 0;
	zombie.SourceLevel = 0;
	zombie.IsNotWalkingFlag = false;
	zombie.GhostFlameMark = 0;

	zombie.VariantType = 0;
	zombie.InvulnerableDuration = 0;
	zombie.ChillDuration = 0;

	zombie.FrostStack = 0;
	zombie.PoisonStack = 0;
	zombie.FlameStack = 0;
	zombie.IceBounded = false;

	zombie.LastDamageSourceID = 0;
	ZombieAbility::GetAbility(zombie.Type)->onCreated(zombie);
}

AnimationType::AnimationType GetZombieReanimType(MyZombie zombie, AnimationType::AnimationType type)
{
	return ZombieAbility::GetAbility(zombie.Type)->GetReanimType(zombie, type);
}

int onPlantTakeDamage(MyPlant plant, PVZ::BaseClass source, GameObjectType::GameObjectType source_type, int damage)
{
	plant.HpDisplayCounter = 100;

	if (plant.Type == SeedType::Spikerock && source_type == GameObjectType::OBJECT_TYPE_NONE && damage == 50)
		damage = 200;

	return damage;
}

ThreeState::ThreeState onZombieSkipEatPlant(MyZombie zombie, MyPlant plant)
{
	switch (plant.Type)
	{
	case SeedType::Iceshroom:
	case SeedType::Blover:
	case SeedType::Squash:
		return ThreeState::Enable;
	case SeedType::Garlic:
	case SeedType::PotatoMine:
		return ThreeState::Disable;
	default:
		break;
	}
	return ThreeState::None;
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
	
	float v = velocity;
	//撑杆僵尸正常跑
	if (zombie.State == ZombieState::POLE_VALUTING_RUNNING)
		v *= 2.0f;
	//橄榄球冲刺加速
	if (zombie.Type == ZombieType::FootballZombie && zombie.HelmType != HelmType::None)
		v += zombie.FootballZombieChargeTime * 0.004f;
	//冰道加速
	if (zombie.X > zombie.GetBoard().GetIcetrace().GetX(zombie.Row) - 40 && zombie.EffectedBy(PVZ::DRF_GROUND))
		v *= 2.0f;
	//击飞或击退则使用+34位移
	if (zombie.IsLaunched)
	{
		v = zombie.Speed;
		if (zombie.ZombieHeight != 7)
		{
			//如果击退，则速度衰减
			zombie.Speed *= 0.95f;
			if (zombie.Speed < 0.05f && zombie.Speed > -0.05f)
			{
				//速度降到阈值后，状态变为正常，重置速度
				zombie.IsLaunched = 0;
				zombie.PickRandomSpeed();
			}
		}
	}

	//撑杆僵尸空中额外位移
	if (zombie.State == ZombieState::POLE_VALUTING_JUMPPING)
		v += 0.5f;//额外总位移：0.5px/cs * 180cs
	//海豚僵尸空中额外位移
	if (zombie.State == ZombieState::DOPHIN_JUMP)
	{
		v += 0.35f;//额外总位移：0.35px/cs * 230cs
	}
	return v;
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
		rate *= FROST_DECELERATE(zombie);
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

/// @deprecated
bool onZombieUpdateColor(MyZombie zombie,PVZ::Animation anim,int red,int green,int blue,int alpha)
{
	if (zombie.Hypnotized && zombie.HpPoint >= 4)
	{
		PVZ::Color color{ 232, 70, 255, alpha };
		if (zombie.HpPoint == 5)
			color = PVZ::Color{232, 21, 255, alpha};
	
		anim.SetColor(color);
		anim.SetAdditiveColor(color);
		anim.DrawAdditiveColor = true;
		return false;
	}
	if (zombie.ChillDuration > 0)
	{
		return true;
	}
	if (zombie.HpPoint == 5)
	{
		PVZ::Color color{ 248,255,0,alpha };
		anim.SetColor(color);
		anim.SetAdditiveColor(color);
		anim.DrawAdditiveColor = true;
		return false;
	}
	if (zombie.HpPoint == 4)
	{
		PVZ::Color color{ 255,144,255,alpha };
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

int onDrawZombieReanim(MyZombie zombie, PVZ::Animation anim)
{
	bool draw_additive_color = anim.DrawAdditiveColor;
	auto original_base_color = anim.GetColor();
	auto original_additive_color = anim.GetAdditiveColor();

	auto colorflag = zombie.ColorFlag;
	if (colorflag)
	{
		float ratio = 0.4f;//取值范围为0.4~1
		switch (colorflag)
		{
		case 1:
			ratio += 0.015f * min(zombie.PoisonStack, 40);
			original_base_color.Red = 100 + (255 - 100) * (1.0f - ratio);
			original_base_color.Blue = 255 * (1.0f - ratio);
			original_additive_color.Green = original_additive_color.Green * (1.0f - ratio) + 255 * ratio;
			original_additive_color.Red = original_additive_color.Red * (1.0f - (100 / 255.0f) * ratio) + 100 * ratio;
			break;
		case 2:
			ratio += 0.02f * zombie.FrostStack;
			original_base_color.Green = 100 + (255 - 100) * (1.0f - ratio);
			original_base_color.Red = 255 * (1.0f - ratio);
			original_additive_color.Blue = original_additive_color.Blue * (1.0f - ratio) + 255 * ratio;
			original_additive_color.Green = original_additive_color.Green * (1.0f - (100 / 255.0f) * ratio) + 100 * ratio;
			break;
		case 3:
			ratio += 0.001f * min(zombie.FlameStack, 600);
			original_base_color.Green = 100 + (255 - 100) * (1.0f - ratio);
			original_base_color.Blue = 255 * (1.0f - ratio);
			original_additive_color.Red = original_additive_color.Red * (1.0f - ratio) + 255 * ratio;
			original_additive_color.Green = original_additive_color.Green * (1.0f - (100 / 255.0f) * ratio) + 100 * ratio;
			break;
		default:
			break;
		}
		draw_additive_color = true;
	}
	anim.SetColor(original_base_color);
	anim.SetAdditiveColor(original_additive_color);
	anim.DrawAdditiveColor = draw_additive_color;

	if (zombie.ShieldType != 0)
		return 1;
	return 0;
}

/// @deprecated
bool onZombieSetColor(MyZombie zombie, PVZ::Animation anim, unsigned int stack_pointer)
{
	bool draw_additive_color = PVZ::Memory::ReadMemory<bool>(stack_pointer + 0x2F);
	PVZ::Color* original_base_color = (PVZ::Color*)((void*)(stack_pointer + 0x50));
	PVZ::Color* original_additive_color = (PVZ::Color*)((void*)(stack_pointer + 0x40));

	auto colorflag = zombie.ColorFlag;
	if (colorflag)
	{
		float ratio = 0.4f;//取值范围为0.4~1
		switch (colorflag)
		{
		case 1:
			ratio += 0.015f * min(zombie.PoisonStack, 40);
			original_base_color->Red = 100 + (255 - 100) * (1.0f - ratio);
			original_base_color->Blue = 255 * (1.0f - ratio);
			original_additive_color->Green = original_additive_color->Green * (1.0f - ratio) + 255 * ratio;
			original_additive_color->Red = original_additive_color->Red * (1.0f - (100 / 255.0f) * ratio) + 100 * ratio;
			break;
		case 2:
			ratio += 0.02f * zombie.FrostStack;
			original_base_color->Green = 100 + (255 - 100) * (1.0f - ratio);
			original_base_color->Red = 255 * (1.0f - ratio);
			original_additive_color->Blue = original_additive_color->Blue * (1.0f - ratio) + 255 * ratio;
			original_additive_color->Green = original_additive_color->Green * (1.0f - (100 / 255.0f) * ratio) + 100 * ratio;
			break;
		case 3:
			ratio += 0.001f * min(zombie.FlameStack, 600);
			original_base_color->Green = 100 + (255 - 100) * (1.0f - ratio);
			original_base_color->Blue = 255 * (1.0f - ratio);
			original_additive_color->Red = original_additive_color->Red * (1.0f - ratio) + 255 * ratio;
			original_additive_color->Green = original_additive_color->Green * (1.0f - (100 / 255.0f) * ratio) + 100 * ratio;
			break;
		default:
			break;
		}
		draw_additive_color = true;
	}
	anim.SetColor(*original_base_color);
	anim.SetAdditiveColor(*original_additive_color);
	anim.DrawAdditiveColor = draw_additive_color;
	return false;
}


bool onZombieUpdateAbility(MyZombie zombie)
{
	return ZombieAbility::GetAbility(zombie.Type)->TickAbility(zombie);
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

	switch (plant.Type)
	{
	case SeedType::Garlic:
		PVZ::ApplyZPDamage(zombie, plant, 50);
		break;
	case SeedType::PotatoMine:
	{
		if (plant.OwnerID == 0 && plant.State == PlantState::POTATO_ARMED)
		{
			PVZ::ApplyZPDamage(zombie, plant, 100);
			//爆炸
			PVZ::Memory::Execute(AsmBuilder().push_imm32(plant.GetBaseAddress()).invoke(0x4666A0).ret());
		}
	}
		break;
	default:
		break;
	}

	return true;
}

void onZombieFinishYuckyFace(MyZombie zombie)
{
	//啃完大蒜后5s中毒
	zombie.AddPoison(30);
}

bool onZombieYuckyFaceChangeRowBefore(MyZombie zombie)
{
	zombie.IsWalkingBackwards = 1;
	return false;
}

void onZombieEatPlant(MyZombie zombie, MyPlant plant)
{
	if (zombie.Type == ZombieType::FootballZombie && zombie.HelmType != HelmType::None)
	{
		//基于头盔生命值和冲刺时间对植物造成眩晕
		//具体公式：冲刺时间*(1+头盔血量百分比)/2
		plant.Stun(zombie.FootballZombieChargeTime*(0.5f+zombie.HelmHealth*0.5f/zombie.HelmMaxHealth));
		//橄榄撞完后掉头盔
		zombie.Hit(zombie.HelmHealth);
		zombie.FootballZombieChargeTime = 0;
	}
}

bool onZombieUpdateEatingAnimSpeed(MyZombie zombie)
{
	if (zombie.Type == ZombieType::NewspaperZombie)
	{
		float rate = 36.0f * (1.0f + zombie.NewspaperAngerStack / 10.0f);
		PVZ::Memory::Execute(AsmBuilder().push_float(rate).mov_reg_imm(REG_EAX, zombie.GetBaseAddress()).invoke(0x52EFF0).ret());
		return false;
	}
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

bool onZombieUpdateFalling(MyZombie zombie)
{
	if (zombie.IsLaunched)
	{
		zombie.Height += zombie.FallSpeed;
		zombie.FallSpeed -= 0.05f;//原版小鬼使用的加速度
		return false;
	}
	return true;
}

void onZombieFallOnGround(MyZombie zombie)
{
	if (zombie.IsLaunched)
	{
		zombie.IsLaunched = 0;
		zombie.FallSpeed = 0.0f;
		zombie.PickRandomSpeed();
	}
}

ThreeState::ThreeState onZombieIsNotWalking(MyZombie zombie)
{
	if (zombie.IsNotWalkingFlag)
		return ThreeState::Enable;
	return ThreeState::None;
}

bool onZombieStartPlayWalkAnim(MyZombie zombie, PVZ::Animation anim,int blendtime)
{
	if (zombie.Type == ZombieType::TallnutZombie && zombie.IsNotWalking())
	{
		zombie.PlayAnim("anim_idle", 12.0f, blendtime, 0);
		return false;
	}
	return true;
}

int onZombieCanTargetPlant(MyZombie zombie, MyPlant plant, int AttackType)
{
	if (zombie.IsLaunched)
		return 0;
	if (zombie.State == ZombieState::DIGGER_WALK_RIGHT && zombie.X < 130)
		return 0;
	if (zombie.Type == ZombieType::TallnutZombie)
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
	//雪橇出场无敌
	if (zombie.State == ZombieState::BOBSLET_ENTER)
	{
		return 0;
	}
	//雪橇车领队不能被直接秒杀
	if (zombie.GetBobsledPosition() == 0)
	{
		dmg = min(dmg, zombie.HelmHealth);
	}
	return dmg;
}

bool onZombiePickRandomSpeed(MyZombie zombie)
{
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
	/*
	switch (zombie.Type)
	{
	case ZombieType::Gargantuar:
	case ZombieType::Gigagargantuar:
		if (PVZ::Memory::ReadMemory<int>(0x701190))
		{
			zombie.VariantType = 1;
			zombie.Speed = Creator::RandFloat(0.14f) + 0.23f + 0.66f;
		}
		if (PVZ::Memory::ReadMemory<int>(0x701200))
			zombie.VariantType = 2;
		return false;
	default:
		break;
	}
	*/
	return true;
}

int onZombieFindTargetInterval(MyZombie zombie)
{
	//existed time最大值约为21亿，即2100W cs，÷100后为210000cs，约58小时
	int cd = zombie.ExistedTime * 100;
	//引入寒意百分比减速
	int interval = 400 / (FROST_DECELERATE(zombie));
	//原版减速，间隔翻倍
	if (zombie.DecelerateCountdown > 0)
		interval *= 2;
	//读报满层怒气最高造成三倍伤害
	if (zombie.Type == ZombieType::NewspaperZombie)
	{
		interval = interval / (1.0f + zombie.NewspaperAngerStack / 10.0f);
	}


	if (cd % interval < 100)
		return 1;
	else return 0;

	return -1;
}

int onZombieEffectedByDamageRange(MyZombie zombie, PVZ::DamageRangeFlags drf)
{
	float height = zombie.Height;
	if (zombie.Type == ZombieType::BalloonZombie)
		height += 10.0f;
	//高空僵尸会被视为飞行范围内
	if (height >= 30.0f)
	{
		if ((drf & PVZ::DRF_FLYING) == 0)
			return 0;
	}
	return -1;
}

bool onPoleVaulterHalfJump(MyZombie zombie, MyPlant plant)
{
	return zombie.FromWave < WAVE_ELITE_MASK;
}

bool onZombieDropArmParticle(MyZombie zombie, PVZ::TodParticleSystem particle_sys)
{
	return ZombieAbility::GetAbility(zombie.Type)->OverrideDropArmParticle(zombie, particle_sys);
}

void onZombieDropHelmParticle(MyZombie zombie, PVZ::TodParticleSystem particle_sys)
{
	ZombieAbility::GetAbility(zombie.Type)->OverrideDropHelmParticle(zombie, particle_sys);
}

bool onZombieUpdateHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
{
	return ZombieAbility::GetAbility(zombie.Type)->OverrideHelmDamageTexture(zombie, reanim, partition);
}

bool OverrideZombieDrawPos(MyZombie zombie, PVZ::ZombieDrawPosition* draw_pos)
{
	return ZombieAbility::GetAbility(zombie.Type)->OverrideDrawPos(zombie, draw_pos);
}

bool onCatapultZombieFire(MyZombie zombie)
{
	for (int num = 0; num < 3; num++)
	{
		int targetaddr = zombie.FindCatapultTarget();
		if (targetaddr != 0)
		{
			zombie.ZombieCatapultFire(targetaddr);
			MyPlant targetplant{ targetaddr };
			targetplant.CatapultTargetSkip = 1;
		}
	}
	auto plants = zombie.GetBoard().GetAllPlants<MyPlant>();
	for (auto& plant : plants)
	{
		plant.CatapultTargetSkip = 0;
	}
	return false;
}

bool onCatapultTargetSkip(MyZombie zombie, MyPlant plant)
{
	if (plant.CatapultTargetSkip)
		return false;
	return true;
}

bool onZombieCheckSquish(MyZombie zombie)
{
	if (zombie.State != ZombieState::CATAPULT_SHOOT && zombie.State != ZombieState::CATAPULT_IDLE)
	{
		auto attack_rect = zombie.GetActualAttackRect();
		auto zombies = zombie.GetBoard().GetAllZombies<MyZombie>();
		for (auto myzombie : zombies)
		{
			if (zombie.Hypnotized == myzombie.Hypnotized || zombie.Row != myzombie.Row)
				continue;
			switch (myzombie.State)
			{
			case ZombieState::DYING:
			case ZombieState::DYING_FROM_INSTANT_KILL:
			case ZombieState::DYING_FROM_LAWNMOWER:
			case ZombieState::BUNGEE_TARGET_DROP:
			case ZombieState::BUNGEE_BODY_DROP:
			case ZombieState::DIGGER_DIG:
			case ZombieState::DIGGER_LOST_DIG:
			case ZombieState::IMP_FLYING:
			case ZombieState::BALLOON_FLYING:
				continue;
			}

			auto rect = myzombie.GetActualRect();
			if (PVZ::GetXOverlap(attack_rect, rect) >= -30)
				myzombie.Hit(1, PVZ::DAMAGEF_HITS_SHIELD_AND_BODY);
		}
	}
	auto rect = zombie.GetActualAttackRect();
	return !(zombie.Hypnotized || zombie.ZombieHeight == 9);
}

void onZombieAddProj(MyZombie zombie, MyProjectile proj)
{
	switch (zombie.Type)
	{
	case ZombieType::GatlingPeaZombie:
		proj.DamageAbility = PVZ::DRF_GROUND | PVZ::DRF_HYPNOTIZED;
		if (zombie.Hypnotized)
		{
			proj.Type = ProjectileType::Pea;
			proj.X += 100;
			proj.DamageAbility = PVZ::DRF_GROUND;
		}
		break;
	case ZombieType::PeashooterZombie:
		proj.DamageAbility = PVZ::DRF_GROUND | PVZ::DRF_HYPNOTIZED;
		if (zombie.Hypnotized)
		{
			proj.Type = ProjectileType::Pea;
			if (zombie.VariantType == ZombieVariantType::SnowPeaHead)
				proj.Type = ProjectileType::SnowPea;
			proj.X += 100;
			proj.DamageAbility = PVZ::DRF_GROUND;
		}
		break;
	}
}

bool IsGatlingZombieShoot(MyZombie zombie)
{
	if (zombie.Hypnotized)
		return zombie.AttributeCountdown == 19 || zombie.AttributeCountdown == 32
			|| zombie.AttributeCountdown == 45 || zombie.AttributeCountdown == 58 || zombie.AttributeCountdown == 71;
	else
		return zombie.AttributeCountdown == 18 || zombie.AttributeCountdown == 35
			|| zombie.AttributeCountdown == 51 || zombie.AttributeCountdown == 68;
}

void onGargantaurThrowAfter(MyZombie zombie, MyZombie imp)
{
	//减速倒计时在之前代码里已经被继承过，无需重复
	imp.FrostStack = zombie.FrostStack;
	imp.UpdateAnimSpeed();
	if (zombie.Hypnotized)
	{
		imp.Hypnotized = true;
		imp.X += 266;
		imp.Speed = -3.0;
	}

	if (zombie.Type == ZombieType::Gargantuar && zombie.BodyMaxHealth >= 4000)
	{
		imp.BodyHealth += 150;
		imp.BodyMaxHealth += 150;
		imp.Size = 1.3f;
		imp.GoldMark = 1;
	}
	if (zombie.Type == ZombieType::Gigagargantuar && zombie.BodyMaxHealth >= 10000)
	{
		imp.BodyHealth += 450;
		imp.BodyMaxHealth += 450;
		imp.Size = 1.5f;
		imp.GoldMark = 1;
	}
}

int onGargantaurJudgeSquish(MyZombie zombie)
{
	//空投巨人不能砸植物
	if (zombie.ZombieHeight == 9)
		return 0;
	//巨人似乎不能直接用这个函数寻找僵尸？
	//if (zombie.FindZombieTarget().isValid())
		//return 1;
	if (zombie.Hypnotized)
		return 0;
	return -1;
}

bool onGargantaurSquishPlant(MyZombie attacker)
{
	if (attacker.Hypnotized)
	{
		auto atk_rect = attacker.GetActualAttackRect();
		int damage = 0;
		if (attacker.Type == ZombieType::Gargantuar)
			damage = attacker.BodyMaxHealth <= 3000 ? 900 : 1200;
		else if (attacker.Type == ZombieType::Gigagargantuar)
			damage = attacker.BodyMaxHealth <= 6000 ? 900 : 1800;

		auto zombies = attacker.GetBoard().GetAllZombies<MyZombie>();
		for (auto myzombie : zombies)
		{
			//为什么是不等于 而不是等于？
			if (attacker.Hypnotized != myzombie.Hypnotized || attacker.Row != myzombie.Row || attacker.Id == myzombie.Id)
				continue;
			auto rect = myzombie.GetActualRect();
			if (PVZ::GetXOverlap(atk_rect, rect) >= -30)
				myzombie.Hit(damage);
		}
	}
	return !attacker.Hypnotized;
}

float onPogoUpdateHeight(MyZombie zombie, float height)
{
	return height;
}

bool onJalapenoHeadBurnBefore(MyZombie zombie)
{
	MyBoard board{ zombie.GetBoard() };
	if (zombie.Hypnotized)
	{
		Creator::CreatePlant(SeedType::Jalapeno, zombie.Row, 20).CreateEffect();
		return false;
	}
	auto zombies = board.GetAllZombies<MyZombie>();
	int count = 0;
	for (auto& zombie_ : zombies)
	{
		if (zombie_.Row == zombie.Row && !zombie.NotExist && !zombie.Hypnotized)
		{
			count += zombie_.BodyHealth;
			count += zombie_.HelmHealth;
			count += zombie_.ShieldHealth;
			if (zombie_.BodyHealth > 1800)
				zombie_.BodyHealth = 1799;
			zombie_.Blast();
		}
	}
	MyZombie summoned = board.AddZombieInRow(ZombieType::Gigagargantuar, zombie.Row, 1);//后续可以考虑传入from wave为精英标记
	float amplify = min(1.0f + count / 5000.0f,2.0f);
	summoned.Size = amplify;
	summoned.BodyHealth *= amplify;
	summoned.BodyMaxHealth *= amplify;
	summoned.RiseFromGrave(zombie.Row,board.PixelToGridX(zombie.X,zombie.Y));
	return false;
}

bool onDiggerZombieUndergroundFindTarget(MyZombie zombie, MyPlant plant)
{
	if (plant.Type == SeedType::Pumpkin)
		return false;
	// 对于会移动的植物，不应该让矿工记录挖根列数
	if (plant.Type == SeedType::Squash)
		return false;
	if (plant.Type == SeedType::Spickweed)
	{
		zombie.DiggerLoseAxe();
		return false;
	}

	if (plant.Column != zombie.DiggerLastDigRootColumn)
	{
		zombie.DiggerLastDigRootColumn = plant.Column;
		// 对本格内所有植物造成伤害
		auto plants = zombie.GetBoard().GetAllPlants<MyPlant>();
		std::vector<MyPlant> dug_plants{};
		bool has_bottom_protect = false;
		bool lose_axe = false;
		for (auto& plant_ : plants)
		{
			if (plant_.Row == plant.Row && plant_.Column == plant.Column)
			{
				switch (plant_.Type)
				{
				case SeedType::Pumpkin:
				case SeedType::CoffeeBean:
				case SeedType::GraveBuster:
				// 可移动植物不会被挖根
				case SeedType::Squash:
				case SeedType::Spickweed:
					break;
				// S6中睡莲被晶钻菇夺舍了，暂时先去掉
				// case SeedType::LilyPad:
				case SeedType::FlowerPot:
					has_bottom_protect = true;
					break;
				case SeedType::Wallnut:
				case SeedType::Tallnut:
				case SeedType::Explodenut:
				case SeedType::Spikerock:
				case SeedType::PotatoMine:
					lose_axe = true;
				default:
					dug_plants.push_back(plant_);
					break;
				}
			}
		}
		if (!has_bottom_protect)
		{
			for (auto& plant_ : dug_plants)
			{
				PVZ::ApplyZPDamage(zombie, plant_, 15);
			}
		}
		if (lose_axe)
			zombie.DiggerLoseAxe();
	}
	//无论挖不挖根，矿工都不应当停下来啃咬植物，因此总是返回false。
	return false;
}

bool onPogoUpdateActions(MyZombie zombie)
{
	/*
	*以下是誊抄的原版代码
	if (zombie.AttributeCountdown != 0)
		return false;

	zombie.AttributeCountdown = 80;
	auto target = zombie.FindPlantTarget(2);
	if (zombie.FromWave == -2 || zombie.FromWave == -3 || !(target.isValid()))
	{
		zombie.State = ZombieState::POGO_WITH_STICK;
		zombie.PickRandomSpeed();
	}
	if (zombie.State == ZombieState::POGO_IDLE_BEFORE_TARGET)
	{
		zombie.Speed = (zombie.ImageX - target.ImageX + 60) / 80.0f;
	}
	else
	{
		zombie.State = ZombieState::POGO_IDLE_BEFORE_TARGET;
		zombie.Speed = 0.0f;
	}
	return false;
	*/
	return true;
}

ThreeState::ThreeState IsZombieCanBeChilled(MyZombie zombie)
{
	switch (zombie.Type)
	{
	case ZombieType::ZombieBobsledTeam:
	case ZombieType::PogoZombie:
	case ZombieType::ZombieYeti:
		return ThreeState::Disable;
	case ZombieType::FootballZombie:
		return zombie.FromWave == WAVE_ELITE1 ? ThreeState::Disable : ThreeState::None;
	default:
		return ThreeState::None;
	}
}

void onZombieChilled(MyZombie zombie)
{
	zombie.DecelerateCountdown = 400;
}

void onZombieRemoveIceTrap(MyZombie zombie)
{
	if (zombie.IceBounded)
	{
		zombie.IceBounded = false;
		zombie.Hit((zombie.BodyHealth + zombie.HelmHealth + zombie.ShieldHealth) >> 3, PVZ::DAMAGEF_BYPASSES_SHIELD);
	}
}

void InitZombieEvents()
{
	// 僵尸初始化相关
	ZombieInitAfterEvent((int)onZombieInitAfter);
	PVZEvent::ZombieGetReanimTypeEvent((int)GetZombieReanimType);
	PVZEvent::LoadPlainZombieReanimBeforeEvent((int)onLoadPlainZombieReanimBefore);

	// 僵尸绘制相关
	DrawZombieReanimEvent((int)onDrawZombieReanim);
	//PVZEvent::ZombieSetColorEvent((int)onZombieSetColor);
	///PVZEvent::ZombieUpdateColorEvent((int)onZombieUpdateColor);
	PVZEvent::ZombieDropArmParticleEvent((int)onZombieDropArmParticle);
	PVZEvent::ZombieDropHelmParticleEvent((int)onZombieDropHelmParticle);
	PVZEvent::ZombieTakeHelmDamageTextureEvent((int)onZombieUpdateHelmDamageTexture);
	PVZEvent::ZombieOverrideDrawPosEvent((int)OverrideZombieDrawPos);

	// 僵尸攻击相关
	PVZEvent::ZombieFindTargetIntervalEvent((int)onZombieFindTargetInterval);
	ZombieTargetPlantEvent((int)onZombieCanTargetPlant);
	PVZEvent::ZombieSquishPlantEvent((int)onZombieSquishPlant);
	PVZEvent::ZombieCheckSquishEvent((int)onZombieCheckSquish);
	PVZEvent::ZombieAddProjectileEvent((int)onZombieAddProj);
	ZombieEatSoundEvent((int)onZombieEatSound);
	PVZEvent::ZombieFinishYuckyFaceEvent((int)onZombieFinishYuckyFace);
	PVZEvent::ZombieYuckyFaceChangeRowBeforeEvent((int)onZombieYuckyFaceChangeRowBefore);
	ZombieEatEvent((int)onZombieEatPlant);
	PVZEvent::ZombieUpdateEatingAnimSpeedEvent((int)onZombieUpdateEatingAnimSpeed);
	PlantTakeDamageEvent((int)onPlantTakeDamage);
	PVZEvent::ZombieSkipEatPlantEvent((int)onZombieSkipEatPlant);

	// 僵尸受击相关
	PVZEvent::ZombieEffectedByDamageRangeEvent((int)onZombieEffectedByDamageRange);
	ZombieTakeDmgEvent((int)onZombieTakeDmg);
	ZombieDropLootEvent((int)onZombieDropLoot);

	// 僵尸移动相关
	PVZEvent::ZombieUpdateWalkingSpeedEvent((int)onZombieUpdateWalkingSpeed);
	PVZEvent::ZombieIsWalkingBackwardsEvent((int)onZombieIsWalkingBackwards);
	PVZEvent::ZombiePickRandomSpeedEvent((int)onZombiePickRandomSpeed);
	PVZEvent::ZombieApplyAnimSpeedEvent((int)onZombieApplyAnimSpeed);
	PVZEvent::ZombieWalkIntoWaterEvent((int)onZombieWalkIntoWater);
	PVZEvent::ZombieWalkOutOfWaterEvent((int)onZombieWalkOutOfWater);
	PVZEvent::ZombieUpdateFallingEvent((int)onZombieUpdateFalling);
	PVZEvent::ZombieFallOnGroundEvent((int)onZombieFallOnGround);
	PVZEvent::ZombieIsNotWalkingEvent((int)onZombieIsNotWalking);
	PVZEvent::ZombieStartPlayWalkAnimEvent((int)onZombieStartPlayWalkAnim);

	// 僵尸特性相关
	ZombieUpdatePlayingEvent((int)onZombieUpdatePlaying);
	PVZEvent::ZombieUpdateAbilityEvent((int)onZombieUpdateAbility);
	// 撑杆僵尸
	PVZEvent::PoleVaulter::HalfJumpEvent((int)onPoleVaulterHalfJump);
	// 小丑僵尸
	PVZEvent::ClownZombiePopEvent((int)onClownZombiePop);
	PVZEvent::HypnotizedClownZombiePopEvent((int)onHypnotizedClownZombiePop);
	// 投篮车僵尸
	PVZEvent::CatapultTargetSkipEvent((int)onCatapultTargetSkip);
	PVZEvent::CatapultZombieFireEvent((int)onCatapultZombieFire);
	// 机枪头僵尸
	PVZEvent::GatlingZombieJudgeShootEvent((int)IsGatlingZombieShoot);
	// 巨人僵尸
	PVZEvent::GargantaurThrowAfterEvent((int)onGargantaurThrowAfter);
	PVZEvent::GargantaurJudgeXFixEvent();
	PVZEvent::GargantaurJudgeSquishEvent((int)onGargantaurJudgeSquish);
	PVZEvent::GargantaurSquishPlantEvent((int)onGargantaurSquishPlant);
	// 跳跳僵尸
	PVZEvent::PogoUpdateHeightEvent((int)onPogoUpdateHeight);
	PVZEvent::PogoUpdateActionsEvent((int)onPogoUpdateActions);
	// 辣椒头僵尸
	PVZEvent::JalapenoHeadBurnBeforeEvent((int)onJalapenoHeadBurnBefore);
	// 矿工僵尸
	PVZEvent::DiggerZombieUndergroundFindTargetEvent((int)onDiggerZombieUndergroundFindTarget);

	PVZEvent::ZombieCanBeChilledEvent((int)IsZombieCanBeChilled);
	PVZEvent::ZombieChillEvent((int)onZombieChilled);
	PVZEvent::ZombieRemoveIceTrapEvent((int)onZombieRemoveIceTrap);

	//修改冰道持续时间
	PVZ::Memory::WriteMemory<int>(0x52A8B6, 1000);
	//矿工正常出土时右行而非左行
	PVZ::Memory::WriteMemory<int>(0x52874E, 0x00000025);
	//矿工从底线出土
	PVZ::Memory::WriteMemory<int>(0x528334, 0x0000041F);
	//海豚随地下水
	static constexpr byte asm_revert_dophin1[] = { 0xEB,0x14,0x90,0x90,0x90 };
	PVZ::Memory::WriteArray<const byte>(0x526211, STRING(asm_revert_dophin1));
	//海豚冲刺到屏幕左侧不出水
	PVZ::Memory::WriteMemory<byte>(0x5263E8, 0xEB);
	//潜水场内下水
	PVZ::Memory::WriteMemory<byte>(0x526747, 0x07);
	//雪橇在冰道外不下车
	PVZ::Memory::WriteMemory<byte>(0x528214, 0xEB);
	//气球是否被DRF_FLYING影响，不再取决于原版的IsFlying函数
	PVZ::Memory::WriteMemory<byte>(0x531C4B, 0xEB);
	PVZ::Memory::WriteMemory<byte>(0x531C4C, 0x0D);
	//覆盖原盲盒开盒
	static constexpr byte asm_revert_1[] = {MOV_PTR_EUX_ADD(REG_EBX, 0x0C4, 0)};
	PVZ::Memory::WriteArray<const byte>(0x530FC4, STRING(asm_revert_1));
	//覆盖原金银特效
	static constexpr byte asm_revert_2[] = { 0x83,0xBE,0xAC,0x00,0x00,0x00,0x00 };
	PVZ::Memory::WriteArray<const byte>(0x52D309, STRING(asm_revert_2));
	//覆盖位于僵尸总更新里的气球落地以及其他代码，此处代码应当搬运至恰当的更新函数中
	static constexpr byte asm_revert_3[] = { 0x83,0xBB,0xFC,0x07,0x00,0x00,0x04 };
	PVZ::Memory::WriteArray<const byte>(0x52AF2F, STRING(asm_revert_3));
	
	// 掉铁桶不露头发
	PVZ::Memory::WriteMemory<WORD>(0x461775, Creator::makeshort(0xEB, 0x38));
	PVZ::Memory::WriteMemory<WORD>(0x530EEA, Creator::makeshort(0xEB, 0x0C));
	//啃完大蒜500cs后才解除YuckyFace
	PVZ::Memory::WriteMemory<int>(0x52B727, 0x000001F4);
}