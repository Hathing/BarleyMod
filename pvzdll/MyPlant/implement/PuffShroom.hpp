#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	inline static const int multiplier_base[6] = { 1, 1, 3, 3, 6, 6 };
	class PuffShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 200;
		}
		bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			rect->X = plant.ImageX + 60;
			rect->Y = plant.ImageY;
			rect->Width = 550;
			rect->Height = plant.Height;
			return true;
		}
		bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			int multiplier = Creator::Rand(10) + multiplier_base[plant.Level];
			proj.SpecialStack = multiplier;
			//如果倍率>=10，记录1，否则记录0
			plant.PuffShroomRecord = plant.PuffShroomRecord << 1;
			if (multiplier >= 10)
			{
				plant.PuffShroomRecord += 1;
			}
			if (plant.Level == 5)
			{
				byte state = plant.PuffShroomRecord & 0x03;
				int time = plant.BodySize > 1.0f ? 10 : 5;
				switch (state)
				{
				case 0:
					//变小
					if (plant.PuffShroomSizeCount > -10)
					{
						plant.PuffShroomSizeCount -= 1;
						plant.SetBodySizeChange(plant.PuffShroomSizeCount > 0 ? -10 : -5);
					}
					break;
				case 3:
					//变大
					if (plant.PuffShroomSizeCount < 10)
					{
						plant.PuffShroomSizeCount += 1;
						plant.SetBodySizeChange(plant.PuffShroomSizeCount < 0 ? 5 : 10);
					}
					break;
				default:
					break;
				}
				float multi = 1.0f;
				if (plant.PuffShroomSizeCount < 0)
					multi += plant.PuffShroomSizeCount * 0.05f;
				else if (plant.PuffShroomSizeCount > 0)
					multi += plant.PuffShroomSizeCount * 0.1f;
				proj.PuffShroomBaseMultiplier = multi;
			}

			return true;
		}
	};
}
