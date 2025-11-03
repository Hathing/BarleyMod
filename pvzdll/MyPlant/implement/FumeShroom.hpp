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
			auto plant = info->plant;
			int distance = info->zombie.X - plant.ImageX - 60;
			distance = min(500, max(0, distance));
			float multi = 1.0f + extra_multiplier[plant.Level] * distance / 500.0f;
			info->damage *= multi;

			if (plant.Level >= 5)
			{
				float prob = 0.5f - 0.001f * distance;
				if (Creator::RandFloat(1.0f) < prob)
				{
					auto proj = MyCreateProjectile(ProjectileType::Puff, plant.Row, 0x4A768, info->zombie.ImageX + 20, info->zombie.ImageY + 40);
					plant.InitAddProjectile(proj);
					proj.SpecialType = PST_SOUL_PUFF;
					proj.Motion = MotionType::Float;
					float v = Creator::RandFloat(8.0f) + 8.0f, theta = Creator::RandFloat(PI * 2);
					proj.XSpeed = v * cosf(theta);
					proj.YSpeed = v * sinf(theta);
					proj.SoulPuffOffsetX = Creator::Rand(20);
					proj.SoulPuffOffsetY = Creator::Rand(20);
				}
			}
		}
		bool onSquishedByZombie(MyPlant plant, MyZombie zombie)
		{
			return PVZ::ApplyZPDamage(zombie, plant, 100);
		}
	};
}
