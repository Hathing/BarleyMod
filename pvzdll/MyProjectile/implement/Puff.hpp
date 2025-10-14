#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Puff : public BaseProjectile
	{
	public:
		inline static const int puffshroom_damage[6] = { 20, 30, 30, 40, 40, 40 };
		inline static const int scaredyshroom_damage[6] = { 20, 20, 30, 30, 40, 40 };
		inline static const int dark_bonus_damage[6] = { 0, 0, 2, 2, 5, 5 };
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			switch (proj.SourceType)
			{
			case SeedType::Puffshroom:
			{
				//special stack表示孢子的倍率
				int dmg = puffshroom_damage[proj.SourceLevel] * proj.SpecialStack;
				if (proj.SourceLevel == 5)
				{
					dmg = max(0, dmg * proj.PuffShroomBaseMultiplier);
				}
				return dmg;
			}
			case SeedType::Fumeshroom:
				return 10;
			case SeedType::Scaredyshroom:
			{
				int total_dmg = scaredyshroom_damage[proj.SourceLevel];

				MyPlant owner = MyPlant::GetByID(proj.ParentID);
				if (owner.isValid())
					total_dmg += (owner.KillCount * 4);

				return total_dmg;
			}
			case SeedType::Seashroom:
			{
				zombie.AddPoison(1);
				return 5;
			}
			case SeedType::DarkShroom:
			{
				int tmp = proj.Unknown;
				int base_damage = 20;
				while (tmp--)
					base_damage >>= 1;
				if (proj.SourceLevel >= MyPlant::MAX_LEVEL && Creator::Rand(5) == 1)
					zombie.AddFrost(1);

				return base_damage + dark_bonus_damage[proj.SourceLevel];
			}
			default:
				return 20;
			}
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SourceType == SeedType::DarkShroom)
			{
				if (proj.Unknown == 0)
					return 0.5f;
				if (proj.Unknown == 2)
					return 0.75f;
			}
			if (proj.SourceType == SeedType::Puffshroom)
			{
				float result = original_val * sqrtf((int)proj.SpecialStack * (proj.SourceLevel == 5 ? proj.PuffShroomBaseMultiplier : 1.0f)) * 0.6f;
				return result;
			}

			return original_val;
		}
	};
}
