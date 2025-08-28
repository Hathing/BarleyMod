#include "pch.h"
#include "MyPlant/PlantAbility.hpp"

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

	PlantAbility::GetPrototype(plant.Type)->onCreated(plant);
}

bool onPlantUpdateAbility(MyPlant plant)
{
	auto tmp = PlantAbility::GetPrototype(plant.Type)->TickAbility(plant);
	if (plant.FromBarley)
	{
		PlantAbility::GetPrototype(SeedType::Barley)->TickAbility(plant);
		if (plant.NotExist)
			return false;
	}
	return tmp;
}

bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
{
	return PlantAbility::GetPrototype(plant.Type)->OverwritePlantAttackRect(plant, secondary, rect);
}

bool onPlantSpecialAnimate(MyPlant plant)
{
	return PlantAbility::GetPrototype(plant.Type)->onAnimate(plant);
}

void onPlantDamageZombie(PZDamageEvent* info)
{
	if (info->type == PVZEvent::PLANTDAMAGETYPE_AOE && info->plant.Type != SeedType::Squash)
	{
		if (info->zombie.NotDying)
			info->zombie.LastDamageSourceID = info->plant.GetOwner().Id;
	}
}

void onPlantDie(MyPlant plant)
{
	PlantAbility::GetPrototype(plant.Type)->onDie(plant);
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
	//如果要跳过原版发射，一定要注意手动重置+58！
	return true;
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
	plant.Fire(PlantWeapon,plant.FindTargetZombie(PlantWeapon));
}

int onPeaShooterSkip(MyPlant plant, MyZombie zombie)
{
	if (plant.Type == SeedType::Peashooter)
	{
		ZombieType::ZombieType zombietype = zombie.Type;
		ZombieState::ZombieState zombiestate = zombie.State;
		if (zombie.NotExist || zombie.ZombieHeight == 9 || zombie.Hypnotized || zombie.Blowaway || !zombie.NotDying)//这里+64是ZombieHeight？我看指针表是僵尸运动状态
			return 1;
		if (zombietype == ZombieType::Zomboin || zombietype == ZombieType::CatapultZombie)
		{
			if (zombie.BodyHealth < 30)
				return 1;
		}
		else if (!zombie.NotDying)
			return 1;
		if (zombietype == ZombieType::BungeeZombie)
			return 1;
		switch (zombiestate)
		{
			case ZombieState::DYING:
			case ZombieState::DYING_FROM_INSTANT_KILL:
			case ZombieState::DYING_FROM_LAWNMOWER:
			case ZombieState::NEWSPAPER_DESTORYED:
			case ZombieState::DIGGER_DIG:
			case ZombieState::DIGGER_LOST_DIG:
			case ZombieState::DIGGER_IDLE:
			case ZombieState::SNORKEL_SWIM:
				return 1;
			default:
				break;
		}
		return 2;
	}
	return 0;
}


byte __asm__StartBlend[24]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	INVOKE(0x473310),
	RET
};
void StartBlend(int blendtime,PVZ::Animation anim)
{
	SETARG(__asm__StartBlend, 1) = blendtime;
	SETARG(__asm__StartBlend, 6) = anim.GetBaseAddress();
	Memory::Execute(STRING(__asm__StartBlend));
}


bool onPlantUpdateShooting(MyPlant plant)
{
	if (plant.Type == SeedType::Peashooter)
	{
		if (plant.ShootingCountdown == 1)
		{
			int targetid = plant.PeashooterTarget;
			if (targetid != 0)
			{
				plant.Fire(0, targetid);
				plant.ShootOrProductCountdown = 300;//真正的重置CD
			}
		}
		plant.ShootingCountdown -= 1;
		if (plant.ShootingCountdown == 0)
		{
			auto anim1 = plant.GetAnimationPart1();
			auto anim2 = plant.GetAnimationPart2();
			if (anim2.isValid() && plant.ShootOrProductInterval > 0)
			{
				StartBlend(20, anim2);
				anim2.SetFramesForLayer("anim_head_idle");
				int base_addr = anim2.GetBaseAddress();
				Memory::WriteMemoryUnsafe<int>((DWORD)base_addr + 0x10, 0);
				anim2.CycleRate = anim1.CycleRate;
				anim2.Speed = anim1.Speed;
			}
			else if (anim1.isValid() && plant.ShootOrProductInterval > 0)
			{
				plant.PlayIdleAnim(anim1.Speed);
			}
			else plant.ShootingCountdown = 1;
		}
		return false;
	}
	if (plant.Type == SeedType::GatlingPea)
	{
		if (plant.AnotherCounter > 0)
		{
			plant.Fire(0, 0);
		}
		else
		{
			if (plant.ShootingCountdown == 16 || plant.ShootingCountdown == 30 || plant.ShootingCountdown == 44 || plant.ShootingCountdown == 58 || plant.ShootingCountdown == 72)
			{
				plant.Fire(0, 0);
			}

		}
		plant.ShootingCountdown -= 1;
		if (plant.ShootingCountdown == 0)
		{
			auto anim1 = plant.GetAnimationPart1();
			auto anim2 = plant.GetAnimationPart2();
			if (anim2.isValid() && plant.ShootOrProductInterval > 0)
			{
				StartBlend(20, anim2);
				anim2.SetFramesForLayer("anim_head_idle");
				int base_addr = anim2.GetBaseAddress();
				Memory::WriteMemoryUnsafe<int>((DWORD)base_addr + 0x10, 0);
				anim2.CycleRate = anim1.CycleRate;
				anim2.Speed = anim1.Speed;
			}
			else if (anim1.isValid() && plant.ShootOrProductInterval > 0)
			{
				plant.PlayIdleAnim(anim1.Speed);
			}
			else plant.ShootingCountdown = 1;
		}
		return false;
	}
	return true;
}

//这是一个测试用的函数
void TakeDamage(int damage, PVZ::DamageFlags flags,int targetid)
{
	SETARG(__asm__Hit, 1) = targetid;
	SETARG(__asm__Hit, 6) = flags;
	SETARG(__asm__Hit, 11) = damage;
	Memory::Execute(STRING(__asm__Hit));
}

bool onPlantAddProjectileBefore(MyPlant plant, ProjectileType::ProjectileType proj_type, int x, int y)
{
	if (plant.Type == SeedType::Peashooter)
	{
		MyZombie zombie{ plant.PeashooterTarget };
		//TakeDamage(500, PVZ::DAMAGEF_NONE, plant.PeashooterTarget);
		//zombie.LastDamageSourceID = plant.GetBaseAddress();//这行代码会崩溃
		zombie.Hit(500, PVZ::DAMAGEF_NONE);
		//创建特效
		auto particle = PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, 0x61A80, EffectType::ZOMBIE_GET_KERNEL_SHOT);
		//particle.OverrideImage(PVZ::Image(0x6A76A8));//这行代码会崩溃
		PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, 0x61A80, EffectType::HAMMER_BANG);
		Creator::CreateLowerSound(LowerSoundType::CherryExplode);
		return false;
	}
	return true;
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
	PVZEvent::PlantAddProjectileBeforeEvent((int)onPlantAddProjectileBefore);
}