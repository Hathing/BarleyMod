#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class GoldSpike : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 5;
		}
	};
	typedef BaseProjectile* ProjectilePTR;
	ProjectilePTR GetAbility(ProjectileType::ProjectileType type);
}
