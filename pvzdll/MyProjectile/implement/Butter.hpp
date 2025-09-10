#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Butter : public BaseProjectile
	{
	public:
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SpecialFlags == 1)
				original_val = 0.72f;
			if (proj.SpecialStack == 1)
				original_val *= 2;
			return original_val;
		}
	};
}
