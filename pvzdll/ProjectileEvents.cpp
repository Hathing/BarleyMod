#include "pch.h"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	proj.ParentID = plant.Id;
	if (plant.Type == SeedType::Starfruit)
		proj.OriginalRow = plant.Row;
	return true;
}

int onProjDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType type, int subtarget_num, int damage)
{
	if (zombie.NotDying && !zombie.NotExist)
	{
		if (type == PVZEvent::ProjDmgType::DAMAGE_SINGULAR)
			zombie.LastDamageSourceID = proj.ParentID;
		else
		{
			auto caster = MyPlant::GetByID(proj.ParentID);
			if (caster.isValid() && caster.Row == zombie.Row)
				zombie.LastDamageSourceID = proj.ParentID;
		}
	}
	return -1;
}

int GetProjectileImage(MyProjectile proj, PVZEvent::ProjectileImgParam param)
{
	if (param == PVZEvent::PROJECTILE_IMAGEROW)
		return 0;
	else
		return -1;
}

void InitProjectileEvents()
{
	PlantAddProjectileEvent((int)onPlantAddProjectile);
	PVZEvent::ProjectileDamageZombieEvent((int)onProjDamageZombie);
	PVZEvent::ProjectileImageEvent((int)GetProjectileImage);
}