#include "pch.h"
#include "MyPlant/PlantAbility.hpp"

void onPlantInitAfter(MyPlant plant)
{
	plant.FromBarley = false;
	plant.OwnerID = 0;
	plant.EasterSkin = false;
	plant.HealCounter = 0;
	plant.AnotherCounter = 0;
	plant.ChillCountdown = 0;
	plant.KillCount = 0;
	plant.Experience = 0;
	plant.Level = 0;
	plant.RelatedPlantID1 = 0;
	plant.RelatedPlantID2 = 0;
	plant.RelatedPlantID3 = 0;
	plant.RelatedPlantID4 = 0;
	plant.SubIndex = 0;
	plant.BarleyCounter = 0;

	auto model = plant.GetAnimationPart2();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
	}
	
	model = plant.GetAnimationPart3();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
	}

	model = plant.GetAnimationPart4();
	if (model.isValid())
	{
		model.AssignRenderGroupToPrefix(-1, "awake");
		model.AssignRenderGroupToPrefix(-1, "easter");
	}

	PlantAbility::GetPrototype(plant.Type)->onCreated(plant);
}

bool onPlantUpdateAbility(MyPlant plant)
{
	auto tmp = PlantAbility::GetPrototype(plant.Type)->TickAbility(plant);
	if (plant.FromBarley)
	{
		PlantAbility::GetPrototype(SeedType::Barley)->TickAbility(plant);
		if (plant.NotExist)
			return false;
	}
	return tmp;
}

bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
{
	return PlantAbility::GetPrototype(plant.Type)->OverwritePlantAttackRect(plant, secondary, rect);
}

bool onPlantSpecialAnimate(MyPlant plant)
{
	return PlantAbility::GetPrototype(plant.Type)->onAnimate(plant);
}

void onPlantDamageZombie(PZDamageEvent* info)
{
	if (info->type == PVZEvent::PLANTDAMAGETYPE_AOE && info->plant.Type != SeedType::Squash)
	{
		if (info->zombie.NotDying)
			info->zombie.LastDamageSourceID = info->plant.GetOwner().Id;
	}
}

void onPlantDie(MyPlant plant)
{
	PlantAbility::GetPrototype(plant.Type)->onDie(plant);
	return;
}

bool onPlantChangeColor(MyPlant plant, PVZ::Animation anim)
{
	if (plant.Type == SeedType::Peashooter)
	{
		anim.SetColor({ 20,255,20,255 });
		return false;
	}

	//默认不修改，返回true
	return true;
}

void InitPlantEvents()
{
	PlantInitAfterEvent((int)onPlantInitAfter);
	PlantUpdateAbilityEvent((int)onPlantUpdateAbility);
	GetPlantAttackRectEvent((int)OverwritePlantAttackRect);
	PVZEvent::PlantSpecialAnimateEvent((int)onPlantSpecialAnimate);
	PVZEvent::PlantDamageZombieEvent((int)onPlantDamageZombie);
	PlantDieEvent((int)onPlantDie);
	PVZEvent::PlantChangeColorEvent((int)onPlantChangeColor);
}