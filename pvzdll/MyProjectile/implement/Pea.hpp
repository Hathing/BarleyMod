#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Pea : public BaseProjectile
	{
	public:
		inline static const int damage[6] = { 20, 20, 20, 20, 20, 20 };
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			int dmg = 20;
			if (proj.SpecialType == PST_SCATTER_PEA)
				dmg /= 2;
			return dmg;
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SpecialType == PST_SCATTER_PEA)
				return original_val / 2.0f;
			return original_val;
		}
	};
}
