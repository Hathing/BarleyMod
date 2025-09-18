#include "pch.h"
#include "MyProjectile/ProjectileAbility.hpp"
#include "MyPlant/PlantAbility.hpp"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	plant.InitAddProjectile(proj);
	if (plant.Type == SeedType::Starfruit)
		proj.OriginalRow = (byte)plant.Row;
	return PlantAbility::GetAbility(plant.Type)->onAddProjectile(plant, proj, zombie);
}

int onProjDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType type, int subtarget_num, int damage)
{
	//这行代码是S6就有的，意义不明，先搬过来
	if (zombie.State == ZombieState::BALLOON_FLYING)
		return -1;

	int mydamage = ProjectileAbility::GetAbility(proj.Type)->OverrideDamage(proj, zombie, type, subtarget_num, damage);
	if (mydamage == 0)
		return 0;

	if (zombie.NotDying && !zombie.NotExist)
	{
		auto caster = MyPlant::GetByID(proj.ParentID).GetOwner();
		if (type == PVZEvent::ProjDmgType::DAMAGE_SINGULAR)
			zombie.LastDamageSourceID = caster.Id;
		else
		{
			if (caster.isValid() && caster.Row == zombie.Row)
				zombie.LastDamageSourceID = caster.Id;
		}
	}
	ProjectileAbility::GetAbility(proj.Type)->onDamageZombie(proj, zombie, type, subtarget_num, mydamage);

	return mydamage;
}

int GetProjectileImage(MyProjectile proj, PVZEvent::ProjectileImgParam param)
{
	if (param == PVZEvent::PROJECTILE_IMAGEROW)
		return 0;
	else
		return ProjectileAbility::GetAbility(proj.Type)->GetImage(proj);
}

float GetProjectileImageSize(MyProjectile proj, float original_val)
{
	return ProjectileAbility::GetAbility(proj.Type)->GetImageSize(proj,original_val);
}

void onProjectileUpdate(MyProjectile proj)
{
	return;
}

bool onProjectileRemove(MyProjectile proj)
{
	//处理弹跳子弹
	if (proj.Motion == MotionType::Throw && proj.BounceCount > 0 && proj.X > 0.0f && proj.X < 1000.0f)
	{
		proj.BounceCount -= 1;
		proj.XSpeed *= 0.45f;
		proj.HeightSpeed *= -0.75f;
		return true;
	}
	return false;
}

bool onProjectileSlideMotion(MyProjectile proj)
{
	//static constexpr int CYCLE_TIME = 50;
	if (proj.SourceType == 18)//三线射手
	{
		proj.YSpeed -= 0.001f * (proj.Y - proj.OriginalY);//简谐运动，对于a = kx,周期T= 2*pi/sqrt(k)
		proj.Y += proj.YSpeed;
		proj.AdjustRow();
		//之前的伪正弦运动，本质上是两段指数运动
		/*
		if (proj.SpecialFlags == PSF_THREEPEATER_SLIDE_OUT)
		{
			proj.YSpeed *= 0.97f;
			if (proj.ExistedTime % CYCLE_TIME == CYCLE_TIME - 1)
			{
				proj.YSpeed *= -1.0f;
				proj.SpecialFlags = PSF_THREEPEATER_SLIDE_IN;
			}
		}
		else if (proj.SpecialFlags == PSF_THREEPEATER_SLIDE_IN)
		{
			proj.YSpeed /= 0.97f;
			if (proj.ExistedTime % CYCLE_TIME == CYCLE_TIME - 1)
			{
				proj.YSpeed *= -1.0f;
				proj.SpecialFlags = PSF_THREEPEATER_SLIDE_OUT;
			}
		}
		*/
		proj.ShadowY += proj.YSpeed;
		return false;
	}
	return true;
}

void onProjectileInitAfter(MyProjectile proj)
{
	proj.ParentID = 0;
	proj.SpecialFlags = 0;
	proj.SpecialStack = 0;
	proj.SpecialType = PST_NONE;
	proj.SourceLevel = 0;
	proj.SourceType = 0;
	proj.BounceCount = 0;
}

void onFireballInitColor(MyProjectile proj, PVZ::Animation anim)
{
	switch (proj.SpecialType)
	{
	case PST_ORANGE_FIREBALL:
		anim.SetColor({ 0xFF,0x70,0x20,0xFF });
		break;
	case PST_RED_FIREBALL:
		anim.SetColor({ 0xFF,0x10,0x10,0xFF });
		break;
	case PST_BLUE_FIREBALL:
		anim.SetColor({ 0x00,0x00,0xFF,0xFF });
		break;
	case PST_PURPLE_FIREBALL:
		anim.SetColor({ 0x70,0x00,0xFF,0xFF });
		break;
	default:
		break;
	}
}

bool IsProjExpire(MyProjectile proj)
{
	return proj.Motion == MotionType::ShortDirect && proj.ExistedTime >= 175;
}

void onPlantAddProjDamageRangeFlags(MyProjectile proj, MyPlant plant)
{
	if (proj.Type == ProjectileType::WinterMelon && proj.SpecialType == PST_CANNON_WINTERMELON)
		proj.DamageAbility = 0;
}

void onProjectileImpact(MyProjectile proj, MyZombie zombie)
{
	//注意，zombie*可能=0，因此一般子弹先要检查zombie是否isValid
	//冰瓜大炮不命中僵尸，也不直接对僵尸操作，因此无需判定
	if (proj.Type == ProjectileType::WinterMelon && proj.SpecialType == PST_CANNON_WINTERMELON)
	{
		auto zombies = proj.GetBoard().GetAllZombies<MyZombie>();
		for (auto& zombie_ : zombies)
		{
			if (zombie_.Row == proj.Row && zombie_.EffectedBy(PVZ::DRF_GROUND | PVZ::DRF_DYING | PVZ::DRF_SUBMERGED) && (zombie_.X<proj.X + 100 && zombie_.X >proj.X - 100))
			{
				int dmg = onProjDamageZombie(proj, zombie_, PVZEvent::DAMAGE_SINGULAR, 0, 0);
				zombie_.Hit(dmg, PVZ::DAMAGEF_HITS_SHIELD_AND_BODY);
				//寒意还没写，暂时用减速代替
				zombie_.Decelerate(1000);
				zombie_.Froze(150);
			}
		}
		//创建音效和特效
		Creator::CreateLowerSound(LowerSoundType::DiamondDrop);
		Creator::CreateLowerSound(LowerSoundType::Decelerate);
		auto effect1 = PVZ::CreateParticleSystem(proj.X - 60.0f, proj.Y + 100.0f, proj.Layer + 100, EffectType::ZOMBOIN_EXPLODED2);
		effect1.OverrideImage(PVZ::Image(Memory::ReadMemory<DWORD>(0x6A72F4)));
		effect1.OverrideScale(2.0f);
		auto effect2 = PVZ::CreateParticleSystem(proj.X - 60.0f, proj.Y + 100.0f, proj.Layer + 100, EffectType::ZOMBOIN_EXPLODED2);
		effect2.OverrideImage(PVZ::Image(Memory::ReadMemory<DWORD>(0x6A764C)));
		effect2.OverrideScale(2.0f);
		PVZ::CreateParticleSystem(proj.X, proj.Y, proj.Layer + 100, EffectType::ICE_SHROOM_EXPLODED);
	}
}

void InitProjectileEvents()
{
	ProjectileRemoveEvent((int)onProjectileRemove);
	PlantAddProjectileEvent((int)onPlantAddProjectile);
	PVZEvent::ProjectileDamageZombieEvent((int)onProjDamageZombie);
	PVZEvent::ProjectileImageEvent((int)GetProjectileImage);
	PVZEvent::ProjectileImageSizeEvent((int)GetProjectileImageSize);
	PVZEvent::ProjectileUpdateEvent((int)onProjectileUpdate);
	PVZEvent::ProjectileSlideMotionEvent((int)onProjectileSlideMotion);
	PVZEvent::ProjectileInitAfterEvent((int)onProjectileInitAfter);
	PVZEvent::FireballInitColorEvent((int)onFireballInitColor);
	PVZEvent::ProjectileCheckExpireEvent((int)IsProjExpire);
	PVZEvent::PlantAddProjDamageRangeFlagsEvent((int)onPlantAddProjDamageRangeFlags);
	PVZEvent::ProjectileImpactEvent((int)onProjectileImpact);
}