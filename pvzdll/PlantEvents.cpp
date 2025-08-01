#include "pch.h"
#include "MyPlant/PlantAbility.hpp"

bool onPlantUpdateAbility(MyPlant plant)
{
	return PlantAbility::GetPrototype(plant.Type)->TickAbility(plant);
}

void InitPlantEvents()
{
	PlantUpdateAbilityEvent((int)onPlantUpdateAbility);
}