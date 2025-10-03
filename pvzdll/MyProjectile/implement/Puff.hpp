#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Puff : public BaseProjectile
	{
	public:
		inline static const int damage[6] = { 20, 20, 30, 30, 40, 40 };
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return damage[proj.SourceLevel];
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
