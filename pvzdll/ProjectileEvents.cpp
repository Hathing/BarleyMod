#include "pch.h"

bool onPlantAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie)
{
	proj.ParentID = plant.Id;
	if(plant.Type == SeedType::Starfruit)
	{
	}
	return true;
}

void InitProjectileEvents()
{
	PlantAddProjectileEvent((int)onPlantAddProjectile);
}