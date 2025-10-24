#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Squash : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 500;
			plant.MaxHp = 500;
			plant.AttributeCountdown = 300;
			plant.SquashBirthX = plant.ImageX;
			plant.SquashBirthLayer = plant.Layer;
		}
		void OverwritePZDamage(PZDamageEvent* info)
		{
			info->flags = PVZ::DAMAGEF_HITS_SHIELD_AND_BODY;
			return;
		}
	};
}
