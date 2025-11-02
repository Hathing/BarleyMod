#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class FumeShroom : public BasePlant
	{
		inline static const float extra_multiplier[6] = { 1.0f, 1.0f, 2.0f, 2.0f, 3.0f, 3.0f };
		inline static const int max_health[6] = { 400, 800, 800, 1200, 1200, 1200 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 150;
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = plant.ImageX + 60;
			rect->Y = plant.ImageY;
			rect->Width = 999999;
			rect->Height = plant.Height;
			return true;
		}
		void OverwritePZDamage(PZDamageEvent* info)
		{
			int distance = info->zombie.X - info->plant.ImageX - 60;
			distance = min(500, max(0, distance));
			float multi = 1.0f + extra_multiplier[info->plant.Level] * distance / 500.0f;
			info->damage *= multi;
			return;
		}
	};
}
