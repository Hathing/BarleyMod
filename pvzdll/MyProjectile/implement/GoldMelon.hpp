#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class GoldMelon : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 1;
		}
		void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			zombie.HitBody(999999);
		}
	};
}
