#pragma once
#include "../ProjectileAbility.hpp"
#include <cmath>

namespace ProjectileAbility
{
	class WinterMelon : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			if (proj.SpecialType == PST_SCATTER_WINTERMELON)
				return 10;//散射冰瓜伤害为范围内无衰减10点
			if (proj.SpecialType == PST_CANNON_WINTERMELON)
				return 120;//冰瓜大炮伤害为120点
			return -1;
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SpecialType == PST_SCATTER_WINTERMELON)
				return original_val * 0.6f;
			else if (proj.SpecialType == PST_CANNON_WINTERMELON)
				return original_val * 2.0f;
			return original_val;
		}
	};
}
