#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Cabbage : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			static const int dmg[6] = {40, 60, 60, 80, 80, 80};

			auto owner = MyPlant::GetByID(proj.ParentID);
			int lvl = 0;
			if (owner.isValid())
				lvl = min(MyPlant::MAX_LEVEL, owner.Level);

			return dmg[lvl];
		}
	};
}
