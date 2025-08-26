#include "pch.h"
#include "MyProjectile/ProjectileAbility.hpp"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	proj.ParentID = plant.Id;
	if (plant.Type == SeedType::Starfruit)
		proj.OriginalRow = (byte)plant.Row;
	if (plant.Type == SeedType::Kernelpult)
		proj.BounceCount = 5;

	return true;
}

int onProjDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType type, int subtarget_num, int damage)
{
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

	return -1;
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
	return ProjectileAbility::GetAbility(proj.Type)->GetImageSize(proj);
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

void InitProjectileEvents()
{
	ProjectileRemoveEvent((int)onProjectileRemove);
	PlantAddProjectileEvent((int)onPlantAddProjectile);
	PVZEvent::ProjectileDamageZombieEvent((int)onProjDamageZombie);
	PVZEvent::ProjectileImageEvent((int)GetProjectileImage);
	PVZEvent::ProjectileImageSizeEvent((int)GetProjectileImageSize);
	PVZEvent::ProjectileUpdateEvent((int)onProjectileUpdate);
}