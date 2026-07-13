#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class SnowPea : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 20;
		}
		void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			zombie.AddFrost(2);
		}
	};
}
