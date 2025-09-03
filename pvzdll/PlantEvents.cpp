#include "pch.h"
#include "MyPlant/PlantAbility.hpp"
#include <cmath>

void onPlantInitAfter(MyPlant plant)
{
	plant.FromBarley = false;
	plant.OwnerID = 0;
	plant.EasterSkin = false;
	plant.HealCounter = 0;
	plant.AnotherCounter = 0;
	plant.ChillCountdown = 0;
	plant.KillCount = 0;
	plant.Experience = 0;
	plant.Level = 0;
	plant.RelatedPlantID1 = 0;
	plant.RelatedPlantID2 = 0;
	plant.RelatedPlantID3 = 0;
	plant.RelatedPlantID4 = 0;
	plant.SubIndex = 0;
	plant.BarleyCounter = 0;

	auto model = plant.GetAnimationPart2();
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

bool onPlantUpdateAbility(MyPlant plant)
{
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
	PlantAbility::GetAbility(info->plant.Type)->OverwritePZDamage(info);
	if (info->type == PVZEvent::PLANTDAMAGETYPE_AOE && info->plant.Type != SeedType::Squash)
	{
		if (info->zombie.NotDying)
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

int onStarFruitFindTarget(MyPlant plant, MyZombie zombie)
{
	return plant.Row == zombie.Row ? 1 : 0;
}

bool onPlantShootMultiple(MyPlant plant)
{
	if (plant.Type == SeedType::Cactus && plant.ShootOrProductCountdown == 50)
		plant.FindTargetAndFire(1);
	if (plant.Type == SeedType::Puffshroom && plant.ShootOrProductCountdown == 50)
		plant.FindTargetAndFire(0);
	if (plant.Type == SeedType::Threepeater && (plant.ShootOrProductCountdown == 35 || plant.ShootOrProductCountdown == 70))
		plant.LaunchThreepeater();
	return PlantAbility::GetAbility(plant.Type)->onShootMultiple(plant);
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
		}
	}
	//清空所有僵尸的PultSkip标记
	auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
	for (auto& zombie : zombies)
	{
		zombie.PultSkip = 0;
	}
}

void onPlantPultMultiple(MyPlant plant,int PlantWeapon)
{
	if (plant.Type == SeedType::Cabbagepult)
	{
		PultMultiple(plant, 3, PlantWeapon);
		return;
	}
	//默认的原版处理，不可改动
	plant.Fire(PlantWeapon,plant.FindTargetZombie(PlantWeapon));
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

int onPlantGetDamageRangeFlags(int PlantWeapon,MyPlant plant)
{
	if (plant.Type == SeedType::Peashooter)
	{
		PVZ::DamageRangeFlags flags = PVZ::DRF_OFF_GROUND | PVZ::DRF_FLYING | PVZ::DRF_GROUND;
		return (int)flags;
	}
	return -1;
}

bool onSingleUsePlantUpdate(MyPlant plant)
{
	if (plant.CanWork > 0)
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
	//概率开大，三线是每发判定概率，而不是每轮（三发）。
	if (plant.ThreepeaterUltraCount==0 && Creator::Rand(3) == 0)
	{
		plant.ThreepeaterUltraCount = 3;
		Creator::CreateUpperSound(UpperSoundType::CoffeeBeanVanish);

		plant.ShootingCountdown = 111 * plant.ThreepeaterUltraCount;
		plant.ShootOrProductCountdown = plant.ShootingCountdown + plant.ShootOrProductInterval;
		return false;
	}
	return true;
}

bool onHypnoShroomEaten(MyZombie zombie, MyPlant plant)
{
	//魅惑菇被啃后掉血
	//这个判断方式，能防止两个僵尸在魅惑菇面前一格互啃的时候扣魅惑的血
	//但是这个判断方式，会导致两个僵尸直接在魅惑菇本格内互啃的时候不扣魅惑的血，有待修复，或者直接当特性也行
	if (zombie.FindZombieTarget().isValid() == false)
	{
		plant.Hp -= 100;
		plant.HpDisplayCounter = 100;
	}
	return false;
}

void InitPlantEvents()
{
	PlantInitAfterEvent((int)onPlantInitAfter);
	PlantUpdateAbilityEvent((int)onPlantUpdateAbility);
	GetPlantAttackRectEvent((int)OverwritePlantAttackRect);
	PVZEvent::PlantSpecialAnimateEvent((int)onPlantSpecialAnimate);
	PVZEvent::PlantDamageZombieEvent((int)onPlantDamageZombie);
	PlantDieEvent((int)onPlantDie);
	PVZEvent::PlantUpdateColorEvent((int)onPlantUpdateColor);
	PVZEvent::StarfruitFindTargetEvent((int)onStarFruitFindTarget);
	PVZEvent::PlantShootMultipleEvent((int)onPlantShootMultiple);
	PVZEvent::PlantPultSkipEvent((int)onPlantPultSkip);
	PVZEvent::PlantPultMultipleEvent((int)onPlantPultMultiple);
	PVZEvent::PlantUpdateShootingEvent((int)onPlantUpdateShooting);
	PVZEvent::PlantFireEvent((int)onPlantFire);
	PVZEvent::PlantFindTargetRTEvent((int)onPlantFindTargetRT);
	PVZEvent::PlantGetDamageRangeFlagsEvent((int)onPlantGetDamageRangeFlags);
	PVZEvent::SingleUsePlantUpdateEvent((int)onSingleUsePlantUpdate);

	PVZEvent::MagnetShroomAttractRadiusEvent((int)onMagnetShroomAttractRadius);
	PVZEvent::MagnetShroomMoveItemEvent((int)onMagnetShroomMoveItem);
	PVZEvent::MagnetShroomAttractItemEvent((int)onMagnetShroomAttractItem);
	PVZEvent::MagnetShroomClearItemEvent((int)onMagnetShroomClearItem);

	PVZEvent::ThreepeaterLaunchEvent((int)onThreepeaterLaunch);

	PVZEvent::HypnoShroomEatenEvent((int)onHypnoShroomEaten);

	//磁力菇只访问+C8 ~ +D8
	PVZ::Memory::WriteMemory<int>(0x461DA6, 1);
	PVZ::Memory::WriteMemory<int>(0x46549C, 1);
	//三线边路不丢豆
	static constexpr byte asm_revert_threepeater1[] = { 0xEB,0x24,0x90};
	PVZ::Memory::WriteArray<const byte>(0x45F40C, STRING(asm_revert_threepeater1));
	static constexpr byte asm_revert_threepeater2[] = { 0xEB,0x21,0x90 };
	PVZ::Memory::WriteArray<const byte>(0x45F383, STRING(asm_revert_threepeater2));
}