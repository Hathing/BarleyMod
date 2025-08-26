#include "pch.h"
#include "MyProjectile/ProjectileAbility.hpp"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	proj.ParentID = plant.Id;
	if (plant.Type == SeedType::Starfruit)
		proj.OriginalRow = plant.Row;
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

void InitProjectileEvents()
{
	PlantAddProjectileEvent((int)onPlantAddProjectile);
	PVZEvent::ProjectileDamageZombieEvent((int)onProjDamageZombie);
	PVZEvent::ProjectileImageEvent((int)GetProjectileImage);
	PVZEvent::ProjectileImageSizeEvent((int)GetProjectileImageSize);
}