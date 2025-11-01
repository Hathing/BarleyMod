#include "pch.h"
#include "MyPlant/PlantAbility.hpp"
#include "MyZombie/ZombieAbility.hpp"
#include <cmath>

void onPlantInitAfter(MyPlant plant)
{
	plant.FromBarley = false;
	plant.RelatedGriditemID1 = 0;
	plant.RelatedGriditemID2 = 0;
	plant.OwnerID = 0;
	plant.EasterSkin = false;
	plant.HealCounter = 0;
	plant.AnotherCounter = 0;
	plant.StunCountdown = 0;
	plant.ChillCountdown = 0;
	plant.Experience = 0;
	plant.Level = 0;
	plant.RelatedPlantID1 = 0;
	plant.RelatedPlantID2 = 0;
	plant.RelatedPlantID3 = 0;
	plant.RelatedPlantID4 = 0;
	plant.SubIndex = 0;
	plant.BarleyCounter = 0;
	plant.FertilizedCounter = 0;
	plant.BodySize = 1.0f;
	plant.BodySizeCountdown = 0;
	plant.HpDisplayCounter = 0;

	auto model = plant.GetAnimationPart1();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
		model.AssignRenderGroupToPrefix(-1, "special");
	}
	
	model = plant.GetAnimationPart2();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
	}
	
	model = plant.GetAnimationPart3();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
	}

	model = plant.GetAnimationPart4();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
	}

	PlantAbility::GetAbility(plant.Type)->onCreated(plant);
}

bool onPlantUpdateAcitveAbilityBefore(MyPlant plant)
{
	if (plant.StunCountdown > 0)
		return false;
	MyBoard board = plant.GetBoard();
	if (!board.EnablePlantAbility)
		return false;

	return true;
}

bool onPlantUpdateAbility(MyPlant plant)
{
	//更新体型
	if (plant.BodySizeCountdown > 0)
	{
		plant.BodySizeCountdown -= 1;
		plant.BodySize += 0.01f;
	}
	else if (plant.BodySizeCountdown < 0)
	{
		plant.BodySizeCountdown += 1;
		if (plant.BodySize > 0.01f)
			plant.BodySize -= 0.01f;
	}

	auto tmp = PlantAbility::GetAbility(plant.Type)->TickAbility(plant);
	if (plant.FromBarley)
	{
		PlantAbility::GetAbility(SeedType::Barley)->TickAbility(plant);
		if (plant.NotExist)
			return false;
	}
	return tmp;
}

bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
{
	return PlantAbility::GetAbility(plant.Type)->OverwritePlantAttackRect(plant, secondary, rect);
}

bool onPlantSpecialAnimate(MyPlant plant)
{
	return PlantAbility::GetAbility(plant.Type)->onAnimate(plant);
}

void onPlantDamageZombie(PZDamageEvent* info)
{
	if(info->type != PVZEvent::PLANTDAMAGETYPE_CUSTOM)
		PlantAbility::GetAbility(info->plant.Type)->OverwritePZDamage(info);
	ZombieAbility::GetAbility(info->zombie.Type)->OverwritePZDamage(info);
	//伤害来源标记
	if (info->zombie.NotDying && info->damage > 0)
	{
		info->zombie.LastDamageSourceID = info->plant.GetOwner().Id;
	}
}

void onPlantDie(MyPlant plant)
{
	PlantAbility::GetAbility(plant.Type)->onDie(plant);
	return;
}

bool onPlantUpdateColor(MyPlant plant, PVZ::Animation anim)
{
	/*
	* 由于很多植物都没写，暂时未实装实际效果
	* 以下是一段范例代码：
	* 
	if (plant.Type == SeedType::Peashooter)
	{
		anim.SetColor({ 20,255,20,255 });
		return false;
	}
	*/

	//默认不修改，返回true
	return true;
}

void onPlantDrawBodySize(MyPlant plant, PVZ::Animation anim)
{
	float bodysize = plant.BodySize;
	anim.XOffset -= (bodysize - 1.0f) * plant.Width * 0.5f;
	anim.YOffset -= (bodysize - 1.0f) * plant.Height;
	anim.XScale *= bodysize;
	anim.YScale *= bodysize;
}

float onPlantDrawBodyStretchRectify(MyPlant plant)
{
	return plant.BodySize * 80.0f;
}

bool onPlantStartBlink(MyPlant plant)
{
	if (plant.Type == SeedType::Squash)
		return false;
	return true;
}

float onPlantDrawShadowSize(MyPlant plant,float original_size)
{
	return original_size * plant.BodySize;
}

int onStarFruitFindTarget(MyPlant plant, MyZombie zombie)
{
	return plant.Row == zombie.Row ? 1 : 0;
}

bool onPlantUpdateShooter(MyPlant plant)
{
	if (plant.Type == SeedType::Cactus && plant.ShootOrProductCountdown == 50)
		plant.FindTargetAndFire(plant.Row, plant.State == PlantState::CACTUS_SHORT_IDLE ? 1 : 0);
	if (plant.Type == SeedType::Threepeater && (plant.ShootOrProductCountdown == 35))
		plant.LaunchThreepeater();
	return PlantAbility::GetAbility(plant.Type)->onUpdateShooter(plant);
}

bool onPlantPultSkip(MyPlant plant,MyZombie zombie)
{
	if (zombie.PultSkip)
	{
		if (plant.Type == SeedType::Cabbagepult || plant.Type == SeedType::Kernelpult || plant.Type == SeedType::Melonpult || plant.Type == SeedType::WinterMelon)
			return false;
	}
	return true;
}

/// @brief 投手同时投出多发子弹
/// @param plant 植物
/// @param num 多发数量
/// @param PlantWeapon 植物副武器
void PultMultiple(MyPlant plant, int num, int PlantWeapon)
{
	for (int i = 0; i < num; i++)
	{
		int targetid = plant.FindTargetZombie(PlantWeapon);
		if (targetid)
		{
			MyZombie target{ targetid };
			plant.Fire(PlantWeapon, targetid);
			target.PultSkip = 1;
			//冰瓜对非主目标发射小冰瓜
			if (plant.Type == SeedType::WinterMelon)
				plant.WinterMelonCastState = 1;
		}
	}
	//清空所有僵尸的PultSkip标记
	auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
	for (auto& zombie : zombies)
	{
		zombie.PultSkip = 0;
	}
	//冰瓜清空散射相关属性
	if (plant.Type == SeedType::WinterMelon)
	{
		plant.WinterMelonCastState = 0;
		plant.WinterMelonScatterCount = 0;
	}
}

static const int wintermelon_scatter_count[6] = { 5, 7, 7, 10, 10, 10 };
static const int kernelpult_scatter_count[6] = { 4, 4, 6, 6, 8, 8 };
void onPlantPultMultiple(MyPlant plant,int PlantWeapon)
{
	switch (plant.Type)
	{
	case SeedType::Cabbagepult:
		PultMultiple(plant, 3, PlantWeapon);
		return;
	case SeedType::WinterMelon:
		{
			int rand_num = Creator::Rand(100);
			if (rand_num < 20)
				plant.WinterMelonScatterCount = wintermelon_scatter_count[plant.Level];
			else if (plant.Level == MyPlant::MAX_LEVEL)
			{
				if (rand_num < 40)
					plant.WinterMelonCastState = 2;
				else
					plant.WinterMelonScatterCount = 5;
			}
			if (plant.WinterMelonScatterCount > 0)
			{
				PultMultiple(plant, plant.WinterMelonScatterCount, PlantWeapon);
				return;
			}
		}
		break;
	case SeedType::Kernelpult:
		{
			if (plant.Level >= 5)
			{
				if (Creator::RandFloat(1.0f) < plant.ButterHitCount * 0.002f)
				{
					plant.ButterHitCount = 0;
					plant.KernelPreparingCannon = true;
					plant.Fire(PlantWeapon, plant.FindTargetZombie(PlantWeapon));
					return;
				}
			}
			int fire_count = kernelpult_scatter_count[plant.Level];
			plant.KernelRandomFire = true;
			while(fire_count>0)
			{
				fire_count--;
				// 最后一发正常弹道
				if (fire_count <= 0)
					plant.KernelRandomFire = false;
				plant.Fire(PlantWeapon, plant.FindTargetZombie(PlantWeapon));
			}
			return;
		}
		break;
	default:
		break;
	}
	//默认的原版处理，不可改动
	plant.Fire(PlantWeapon,plant.FindTargetZombie(PlantWeapon));
}

void onPlantPultSetSpeedAfter(MyPlant plant, MyProjectile proj)
{
	if (plant.Type == SeedType::Kernelpult)
	{
		if (plant.KernelRandomFire)
		{
			proj.XSpeed *= Creator::RandFloat(0.4f) + 0.8f;
			proj.HeightSpeed *= Creator::RandFloat(0.1f) + 0.95f;
		}
	}
}

void onPlantFindTargetResult(MyPlant plant, MyZombie zombie)
{
	if (zombie.isValid())
	{
		//机枪索敌成功后判定开大
		if (plant.Type == SeedType::GatlingPea && plant.AnotherCounter <= 0)
		{
			float ultra_rate = 0.2f;
			if (Creator::RandFloat(1.0f) < ultra_rate)
			{
				plant.UltraCount = 1;
			}
		}
		if (plant.Type==SeedType::SplitPea)
		{
			if (plant.AnotherCounter <= 0)
			{
				if (plant.Level == 5 && plant.FindTargetCount == 1 && Creator::RandFloat(1.0f) < 0.2f)
				{
					plant.UltraCount = 1;
				}
			}
			/*
			else
			{
				plant.ShootOrProductCountdown += 40;//大招间隔0.4s
				plant.Fire(0, zombie.GetBaseAddress());
			}
			*/
		}
	}
	switch (plant.Type)
	{
	case SeedType::SplitPea:
		plant.FindTargetCount +=1;
		break;
	default:
		break;
	}
}

bool onPlantUpdateShooting(MyPlant plant)
{
	return PlantAbility::GetAbility(plant.Type)->onUpdateShooting(plant);
}

//这是一个测试用的函数
void TakeDamage(int damage, PVZ::DamageFlags flags,int targetid)
{
	SETARG(__asm__Hit, 1) = targetid;
	SETARG(__asm__Hit, 6) = flags;
	SETARG(__asm__Hit, 11) = damage;
	Memory::Execute(STRING(__asm__Hit));
}

bool onPlantFire(MyPlant plant, MyZombie target, int weapon_type)
{
	return PlantAbility::GetAbility(plant.Type)->onFire(plant, target, weapon_type);
}

int onPlantFindTargetRT(MyPlant plant,MyZombie zombie,int row)
{
	return PlantAbility::GetAbility(plant.Type)->onFindTargetRT(plant,zombie,row);
}

int GetPlantDamageRangeFlags(MyPlant plant, int weapon)
{
	return PlantAbility::GetAbility(plant.Type)->GetDamageRangeFlags(plant, weapon);
}

bool onSingleUsePlantUpdate(MyPlant plant)
{
	if (plant.SubClass > 0)
	{
		return false;
	}
	return true;
}

int onMagnetShroomAttractRadius(MyPlant plant,MyZombie zombie)
{
	return 800;
}

bool onMagnetShroomMoveItem(MyPlant plant)
{
	//以下内容抄CT
	if (plant.AnotherCounter > 0)
	{
		plant.AnotherCounter -= 1;
	}
	else if (plant.MagnetTarget)
	{
		plant.MagnetState = 1;
		//设置初速度
		//plant.MagnetItemXSpeed
	}
	else
		plant.MagnetState = 0;

	auto item = plant.GetMagnetItem(0);
	if (item.Type != MagnetItemType::None)
	{
		int targetid = plant.MagnetTarget;
		float dx = item.X - plant.ImageX, dy = item.Y - plant.ImageY, vx = plant.MagnetItemXSpeed, vy = plant.MagnetItemYSpeed;
		if (targetid != 0 && plant.MagnetState != 0)
		{
			MyZombie target{ targetid };
			dx = item.X - target.X; 
			dy = item.Y - target.Y;
			if ((dx > 0 && target.X > plant.ImageX) || (dx < 0 && target.X < plant.ImageX))
			{
				//音效
				Creator::CreateLowerSound(LowerSoundType::IronAccessoryHit);
				Creator::CreateLowerSound(LowerSoundType::HammerHit);
				//击退
				if (target.X > plant.ImageX)
					target.X += 20;
				else
					target.X -= 20;
				plant.MagnetItemXSpeed *= -1;
				plant.MagnetItemYSpeed *= -1;
				plant.MagnetState = 0;
				plant.AnotherCounter = 150;//重置CD
			}
		}
		float ax = -0.004f * dx;
		float ay = -0.004f * dy;
		//y坐标阻尼
		if (dy > 40.0f || dy < -40.0f)
			ay += -0.05f * vy;
		//x坐标阻尼
		if (dx > 50.0f || dx < -50.0f)
			ax += -0.05f * vx;
		plant.MagnetItemXSpeed += ax;
		plant.MagnetItemYSpeed += ay;
		item.X += plant.MagnetItemXSpeed;
		item.Y += plant.MagnetItemYSpeed;
	}
	return false;
}

void onMagnetShroomAttractItem(MyPlant plant, MyZombie zombie)
{	
	plant.AttributeCountdown = 10000;
	plant.MagnetState = 0;
	plant.AnotherCounter = 100;//重置CD
	return;
}
void onMagnetShroomClearItem(MyPlant plant)
{
	plant.MagnetItemXSpeed = 0.0f;
	plant.MagnetItemYSpeed = 0.0f;
	return;
}

bool onThreepeaterLaunch(MyPlant plant)
{
	static const float probability[6] = { 0.01f,0.02f,0.02f,0.03f,0.03f,0.03f };
	if (plant.AnotherCounter <= 0 && plant.ShootOrProductCountdown > 0 && Creator::RandFloat(1.0f) < probability[plant.Level])
	{
		int ultra_count = 1;
		if (plant.Level == 5)
			ultra_count = 3;
		int ultra_time = 111 * ultra_count;
		plant.AnotherCounter = ultra_time;
		plant.ShootingCountdown = plant.AnotherCounter;
		plant.ShootOrProductCountdown += plant.AnotherCounter;
		Creator::CreateUpperSound(UpperSoundType::CoffeeBeanVanish);
		return false;
	}
	return true;
}

bool onHypnoShroomEaten(MyPlant plant, MyZombie zombie)
{
	PVZ::ApplyZPDamage(zombie, plant, 100);
	return false;
}

bool onTangleKelpKillZombie(MyPlant plant)
{
	//MyZombie zombie = PVZ::GetByID<MyZombie>((unsigned int)plant.mTargetZombieID);
	//zombie.RemoveWithLoot();
	//移除僵尸的动画附件
	//PVZ::Animation(zombie.HitReanimID).Die();
	plant.State = PlantState::IDLE;
	plant.mTargetZombieID = 0;
	plant.AnotherCounter = 0;
	//由于事件没配好，返回true的话会直接崩溃，S7的水草实际上也不过原版更新，只是利用这个事件在意外状况下重置水草状态，防止水草死亡，就先这样吧
	return false;
}

void onTangleKelpUpdateGrabbing(MyPlant plant)
{
	MyZombie zombie = PVZ::GetByID<MyZombie>((unsigned int)plant.mTargetZombieID);
	if (!zombie.isValid() || zombie.NotExist || !zombie.NotDying || zombie.Hypnotized || zombie.Blowaway)
	{
		//僵尸消失则重置水草状态
		plant.State = PlantState::IDLE;
		plant.mTargetZombieID = 0;
		plant.AttributeCountdown = 0;
		//重置冷却时间为4s
		plant.AnotherCounter = 400;
		return;
	}
	//水草抓住正常僵尸时，锁定自己的+54倒计时，这意味着水草实际上不会经过将僵尸拖下水的特效音效处理（+54=50、+54=20）和死亡处理（+54=0）。
	plant.AttributeCountdown = 100;//此处代码已经移至水草的TickAbility中。
	if (plant.TangleKelpDoEffectCountdown <= 0)
	{
		//水草每1s为僵尸施加5层毒
		plant.TangleKelpDoEffectCountdown = 100;
		zombie.LastDamageSourceID = plant.Id;
		zombie.AddPoison(5);
	}
}

void onTangleKelpTargetAfter(MyPlant plant, MyZombie zombie)
{
	if (zombie.Eating)
	{
		zombie.StopEating();
	}
}

int GetPlantFindTargetZombiePriority(MyPlant plant, MyZombie zombie, int original_priority)
{
	return PlantAbility::GetAbility(plant.Type)->GetTargetZombiePriority(plant, zombie, original_priority);
}

static const int KernelPultProcPartition[6] = {5, 4, 4, 3, 3, 3};
bool IsKernelPultCastButter(MyPlant plant)
{
	return Creator::Rand(KernelPultProcPartition[plant.Level]) == 0;
}

ProjectileType::ProjectileType OverrideProjectileType(MyPlant plant, int PlantWeapon)
{
	return PlantAbility::GetAbility(plant.Type)->OverrideProjectileType(plant, PlantWeapon);
}

bool onPlantDying(MyPlant plant, PlantDyingType dying_type)
{
	bool dying = PlantAbility::GetAbility(plant.Type)->onDying(plant, dying_type);
	if (dying)
	{
		auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
		for (auto& myplant : plants)
			if (myplant.Row == plant.Row && myplant.Id != plant.Id)
				dying = PlantAbility::GetAbility(myplant.Type)->onPlantDying(myplant, plant, dying_type);
	}
	return dying;
}

int onPlantReload(MyPlant plant, int shoot_cd)
{
	return PlantAbility::GetAbility(plant.Type)->Reload(plant, shoot_cd);
}

bool onScaredyShroomScared(MyPlant plant)
{;
	return true;
}

bool onScaredyShroomGrow(MyPlant plant)
{
	if (plant.Level < MyPlant::MAX_LEVEL)
		plant.AddExperience(3000);
	return true;
}

bool onScaredyShroomJudgeZombieNear(MyPlant plant, bool IsZombieNear)
{
	if (plant.Level >= 5)
		return false;
	return IsZombieNear;
}

static const int squash_jump_time[6] = { 4,4,6,6,8,8 };
bool onSquashFallOnGround(MyPlant plant)
{
	//重置状态
	plant.State = PlantState::IDLE;
	plant.mTargetZombieID = 0;
	plant.PlayIdleAnim(12.0f);
	
	//设置跳跃次数
	plant.SquashJumpCount -= 1;
	//最后一跳，重置跳跃次数和图层
	if (plant.SquashJumpCount < 0)
	{
		plant.SquashJumpCount = squash_jump_time[plant.Level];
		plant.Layer = plant.SquashBirthLayer;
	}
	//设置CD
	if (plant.SquashJumpCount == 0)
		plant.AttributeCountdown = 400 + Creator::Rand(201);
	else
		plant.AttributeCountdown = 10;

	return true;
}

int onSquashJumpStartPositionX(MyPlant plant)
{
	return plant.ImageX;
}

bool onSquashFindJumpTarget(MyPlant plant)
{
	if (plant.AttributeCountdown == 0)
		return true;
	return false;
}

bool onSquashSetJumpState(MyPlant plant)
{
	//索敌失败
	if (plant.mTargetZombieID == 0)
	{
		if (plant.ImageX < 760)
		{
			plant.mTargetX = plant.ImageX + 80;//向前跳1格
		}
		else
		{
			plant.mTargetX = 760;
		}
	}

	if (plant.SquashJumpCount == 0)
		return true;

	plant.State = PlantState::SQUASH_LOOK;
	plant.AttributeCountdown = 1;

	if (plant.SquashJumpCount == 1)
		plant.mTargetX = plant.SquashBirthX;

	return false;
}


const float potato_particle_scale[6] = { 1.0f,1.0f,1.3f,1.3f,1.7f,1.7f };
bool onPotatoDieFromExplosion(MyPlant plant, PVZ::TodParticleSystem particle)
{
	if (plant.Type == SeedType::PotatoMine)
	{
		particle.OverrideScale(potato_particle_scale[plant.Level]);
		if (plant.OwnerID == 0)
		{
			plant.AttributeCountdown = 1;
			plant.State = PlantState::IDLE;
			plant.PlayIdleAnim(12.0f);
			return false;
		}
	}
	return true;
}

bool onPotatoExplode(MyPlant plant)
{
	auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
	PVZ::Rect attack_rect;
	plant.GetPlantAttackRect(0, attack_rect);
	int damage = 1800;
	if (plant.OwnerID != 0)
		damage = 600;
	for (auto& zombie : zombies)
	{
		if (zombie.Row == plant.Row && zombie.EffectedBy(PVZ::DRF_GROUND | PVZ::DRF_UNDERGROUND | PVZ::DRF_SUBMERGED | PVZ::DRF_OFF_GROUND | PVZ::DRF_DYING))
		{
			auto zombie_rect = zombie.GetActualRect();
			if (PVZ::GetXOverlap(zombie_rect, attack_rect) >= 0)
				PVZ::ApplyPZDamage(plant, zombie, damage, PVZ::DAMAGEF_HITS_SHIELD_AND_BODY);
		}
	}
	return false;
}

bool onPotatoFindTargetAfter(MyPlant plant, MyZombie zombie)
{
	// 无论是大雷还是小雷，不再依靠原版索敌引爆，而是主动调用4666A0引爆
	return false;
}

bool onTorchwoodFindProjectileBefore(MyPlant plant)
{
	if (plant.AttributeCountdown > 0)
		return false;
	return true;
}

bool onTorchwoodFindProjectile(MyPlant plant, MyProjectile proj)
{
	if (plant.AttributeCountdown > 0)
		return false;
	
	float probability = Creator::RandFloat(1.0f);
	int level = plant.Level;
	bool high_class_convert = false;

	switch (proj.Type)
	{
	case ProjectileType::Pea:
		//豌豆过火
		high_class_convert = (level < 2 ? false : (probability < (level < 4 ? 0.1f : 0.2f)));
		if (high_class_convert && proj.SpecialType == PST_NONE)
			proj.SpecialType = PST_ORANGE_FIREBALL;
		PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EAX, plant.Column).mov_reg_imm(REG_ECX, proj.GetBaseAddress()).invoke(0x46ECB0).ret());
		break;
	case ProjectileType::SnowPea:
		//冰豌豆过火
		PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EBX, plant.Column).mov_reg_imm(REG_EAX, proj.GetBaseAddress()).invoke(0x46EE00).ret());
		break;
	case ProjectileType::FirePea:
		//火球过火
		high_class_convert = (level < 2 ? false : (probability < (level < 4 ? 0.2f : 0.4f)));
		if (high_class_convert && proj.SpecialType == PST_NONE && proj.LastOnFireColumn != plant.Column)
		{
			proj.LastOnFireColumn = plant.Column;
			proj.SpecialType = PST_ORANGE_FIREBALL;
			//修改颜色
			auto attachment = PVZ::GetByID<PVZ::Attachment>(proj.AttachmentID);
			onFireballInitColor(proj, attachment.GetAnimation());
		}
		else
		{
			return false;
		}
		break;
	default:
		//非可过火子弹直接RET
		return false;
	}
	//可过火子弹，如果成功过火，触发火炬过火效果
	plant.AttributeCountdown = 5;


	return false;
}

bool onTorchwoodConvertProjectile(MyPlant plant, MyProjectile proj)
{
	plant.AttributeCountdown += 5;
	return true;
}

void InitPlantEvents()
{
	// 植物初始化与销毁相关
	PlantInitAfterEvent((int)onPlantInitAfter);
	PlantDieEvent((int)onPlantDie);

	// 植物绘制与动画相关
	PVZEvent::PlantSpecialAnimateEvent((int)onPlantSpecialAnimate);
	PVZEvent::PlantUpdateColorEvent((int)onPlantUpdateColor);
	PVZEvent::PlantDrawBodySizeEvent((int)onPlantDrawBodySize);
	PVZEvent::PlantDrawShadowSizeEvent((int)onPlantDrawShadowSize);
	PVZEvent::PlantDrawBodyStretchRectifyEvent((int)onPlantDrawBodyStretchRectify);
	//PVZEvent::PlantStartBlinkEvent((int)onPlantStartBlink);

	// 植物攻击相关
	GetPlantAttackRectEvent((int)OverwritePlantAttackRect);
	PVZEvent::PlantGetDamageRangeFlagsEvent((int)GetPlantDamageRangeFlags);
	PVZEvent::PlantUpdateShooterEvent((int)onPlantUpdateShooter);
	//PlantReloadEvent((int)onPlantReload);
	PVZEvent::PlantNewReloadEvent((int)onPlantReload);
	PVZEvent::PlantFindTargetRTEvent((int)onPlantFindTargetRT);
	PVZEvent::StarfruitFindTargetEvent((int)onStarFruitFindTarget);
	PVZEvent::PlantFindTargetResultEvent((int)onPlantFindTargetResult);
	PVZEvent::PlantUpdateShootingEvent((int)onPlantUpdateShooting);
	PVZEvent::PlantFireEvent((int)onPlantFire);
	PVZEvent::PlantPultSkipEvent((int)onPlantPultSkip);
	PVZEvent::PlantPultMultipleEvent((int)onPlantPultMultiple);
	PVZEvent::PlantDamageZombieEvent((int)onPlantDamageZombie);
	PVZEvent::OverridePlantProjectileTypeEvent((int)OverrideProjectileType);

	// 植物受击死亡相关
	PVZEvent::PlantDyingEvent((int)onPlantDying);

	// 植物特性相关
	PVZEvent::PlantUpdateAcitveAbilityBeforeEvent((int)onPlantUpdateAcitveAbilityBefore);
	PlantUpdateAbilityEvent((int)onPlantUpdateAbility);
	PVZEvent::SingleUsePlantUpdateEvent((int)onSingleUsePlantUpdate);
	// 磁力菇
	PVZEvent::MagnetShroomAttractRadiusEvent((int)onMagnetShroomAttractRadius);
	PVZEvent::MagnetShroomMoveItemEvent((int)onMagnetShroomMoveItem);
	PVZEvent::MagnetShroomAttractItemEvent((int)onMagnetShroomAttractItem);
	PVZEvent::MagnetShroomClearItemEvent((int)onMagnetShroomClearItem);
	// 三线射手
	PVZEvent::ThreepeaterLaunchEvent((int)onThreepeaterLaunch);
	// 魅惑菇
	PVZEvent::HypnoShroomEatenEvent((int)onHypnoShroomEaten);
	// 缠绕海草
	PVZEvent::TangleKelpKillZombieEvent((int)onTangleKelpKillZombie);
	PVZEvent::TangleKelpUpdateGrabbingEvent((int)onTangleKelpUpdateGrabbing);
	PVZEvent::TangleKelpTargetAfterEvent((int)onTangleKelpTargetAfter);
	// 玉米投手
	PVZEvent::KernelPult::JudgeButterEvent((int)IsKernelPultCastButter);
	PVZEvent::PlantPultSetSpeedAfterEvent((int)onPlantPultSetSpeedAfter);
	// 胆小菇
	PVZEvent::ScardyShroomScaredEvent((int)onScaredyShroomScared);
	PVZEvent::ScardyShroomGrowEvent((int)onScaredyShroomGrow);
	PVZEvent::ScardyShroomJudgeZombieNearEvent((int)onScaredyShroomJudgeZombieNear);
	// 倭瓜
	PVZEvent::SquashFallOnGroundEvent((int)onSquashFallOnGround);
	PVZEvent::SquashJumpStartPositionXEvent((int)onSquashJumpStartPositionX);
	PVZEvent::SquashFindJumpTargetEvent((int)onSquashFindJumpTarget);
	PVZEvent::SquashSetJumpStateEvent((int)onSquashSetJumpState);
	// 土豆雷
	PVZEvent::PotatoDieFromExplosionEvent((int)onPotatoDieFromExplosion);
	PVZEvent::PotatoExplodeEvent((int)onPotatoExplode);
	PVZEvent::PotatoFindTargetAfterEvent((int)onPotatoFindTargetAfter);
	// 火炬树桩
	PVZEvent::TorchwoodFindProjectileBeforeEvent((int)onTorchwoodFindProjectileBefore);
	PVZEvent::TorchwoodFindProjectileEvent((int)onTorchwoodFindProjectile);

	//PVZEvent::PlantFindTargetZombiePriorityEvent((int)GetPlantFindTargetZombiePriority);

	//磁力菇只访问+C8 ~ +D8
	PVZ::Memory::WriteMemory<int>(0x461DA6, 1);
	PVZ::Memory::WriteMemory<int>(0x46549C, 1);
	//三线边路不丢豆
	static constexpr byte asm_revert_threepeater1[] = { 0xEB,0x24,0x90};
	PVZ::Memory::WriteArray<const byte>(0x45F40C, STRING(asm_revert_threepeater1));
	static constexpr byte asm_revert_threepeater2[] = { 0xEB,0x21,0x90 };
	PVZ::Memory::WriteArray<const byte>(0x45F383, STRING(asm_revert_threepeater2));
	//水草可以拉陆地僵尸
	PVZ::Memory::WriteMemory<byte>(0x4677A6, 0xEB);
	//倭瓜不会二次索敌
	PVZ::Memory::WriteMemory<byte>(0x460B15, 0xEB);
	//植物不再会根据更新+130调用SetSleeping
	PVZ::Memory::WriteMemory<byte>(0x46320C, 0xEB);
	//植物不再会根据更新+130播放音效
	PVZ::Memory::WriteMemory<byte>(0x4631B1, 0xEB);
	//寒冰菇被碾压时不再爆炸
	PVZ::Memory::WriteMemory<byte>(0x462BF1, 0x18);
	//毁灭菇不留痕
	PVZ::Memory::WriteMemory<int>(0x466887, 1);
}