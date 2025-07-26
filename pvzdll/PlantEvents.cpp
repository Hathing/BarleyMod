#include "pch.h"
#include "MyPlant/PlantAbility.hpp"
#include "../pvzclass/Events/PlantUpdateAbilityEvent.hpp"

bool onPlantUpdateAbility(MyPlant plant)
{
	if (plant.Type > 53)
		return true;
	return PlantAbility::GetPrototype(plant.Type)->TickAbility(plant);
}

void InitPlantEvents()
{
	PlantUpdateAbilityEvent((int)onPlantUpdateAbility);
}