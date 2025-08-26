#pragma once
#include "../ProjectileAbility.hpp"
#include <cmath>

namespace ProjectileAbility
{
	class Melon : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 80 + proj.SpecialStack * 20;
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			return original_val*sqrtf(1.0f+proj.SpecialStack*0.25f);
		}
	};
}
