#include "pch.h"
#include "MyPlant/PlantAbility.hpp"

bool onPlantUpdateAbility(MyPlant plant)
{
	return PlantAbility::GetPrototype(plant.Type)->TickAbility(plant);
}

bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
{
	return PlantAbility::GetPrototype(plant.Type)->OverwritePlantAttackRect(plant, secondary, rect);
}

bool onPlantSpecialAnimate(MyPlant plant)
{
	return PlantAbility::GetPrototype(plant.Type)->onAnimate(plant);
}

void InitPlantEvents()
{
	PlantUpdateAbilityEvent((int)onPlantUpdateAbility);
	GetPlantAttackRectEvent((int)OverwritePlantAttackRect);
	PVZEvent::PlantSpecialAnimateEvent((int)onPlantSpecialAnimate);
}