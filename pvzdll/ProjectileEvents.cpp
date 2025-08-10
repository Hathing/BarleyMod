#include "pch.h"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	proj.ParentID = plant.Id;
	if(plant.Type == SeedType::Starfruit)
	{
	}
	return true;
}

int onProjDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType type, int subtarget_num, int damage)
{
	if (zombie.NotDying && !zombie.NotExist)
		zombie.LastDamageSourceID = proj.ParentID;
	return -1;
}

void InitProjectileEvents()
{
	PlantAddProjectileEvent((int)onPlantAddProjectile);
	PVZEvent::ProjectileDamageZombieEvent((int)onProjDamageZombie);
}