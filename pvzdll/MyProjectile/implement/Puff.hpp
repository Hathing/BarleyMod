#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Puff : public BaseProjectile
	{
	public:
		inline static const int damage[6] = { 20, 20, 30, 30, 40, 40 };
		inline static const int dark_bonus_damage[6] = { 0, 0, 2, 2, 5, 5 };
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			switch (proj.SourceType)
			{
			case SeedType::Puffshroom:
				return damage[proj.SourceLevel] * proj.Unknown2;
			case SeedType::Fumeshroom:
				return 10;
			case SeedType::Scaredyshroom:
			{
				int total_dmg = damage[proj.SourceLevel];

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
				int sz = proj.Unknown2 ;
				
				MyPlant owner = MyPlant::GetByID(proj.ParentID);
				if (owner.isValid())
					sz += owner.KillCount;

				return 1 + sz / 10.0f;
			}

			return original_val;
		}
	};
}
