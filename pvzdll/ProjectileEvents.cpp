#include "pch.h"
#include "MyProjectile/ProjectileAbility.hpp"
#include "MyPlant/PlantAbility.hpp"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	proj.ParentID = plant.Id;
	if (plant.Type == SeedType::Starfruit)
		proj.OriginalRow = (byte)plant.Row;
	if (plant.Type == SeedType::Kernelpult)
		proj.BounceCount = 5;
	return PlantAbility::GetPrototype(plant.Type)->onAddProjectile(plant, proj, zombie);
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
	static constexpr int CYCLE_TIME = 50;
	if (proj.SpecialFlags)
	{
		proj.Y += proj.YSpeed;
		proj.AdjustRow();
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
		proj.ShadowY += proj.YSpeed;
		return false;
	}
	return true;
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
}