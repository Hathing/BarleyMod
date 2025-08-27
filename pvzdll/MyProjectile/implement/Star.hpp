#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Star : public BaseProjectile
	{
	public:
		inline static const int damage[6] = { 20, 20, 30, 30, 40, 40 };
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return damage[proj.SourceLevel];
		}
	};
}
