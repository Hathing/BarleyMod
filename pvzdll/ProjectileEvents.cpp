#include "pch.h"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	if(plant.Type == SeedType::Starfruit)
	{
		proj.ParentID = plant.Id;
	}
	return true;
}

void InitProjectileEvents()
{
	PlantAddProjectileEvent((int)onPlantAddProjectile);
}