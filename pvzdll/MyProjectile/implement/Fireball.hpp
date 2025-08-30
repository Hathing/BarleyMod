#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Fireball : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return ori_dmg;
		}
		void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			if(proj.SpecialType==PST_NONE)
				zombie.AddFlame(20);
			if (proj.SpecialType == PST_ORANGE_FIREBALL)
				zombie.AddFlame(40);
			if (proj.SpecialType == PST_RED_FIREBALL)
				zombie.AddFlame(60);
			if (proj.SpecialType == PST_BLUE_FIREBALL)
				zombie.AddFrost(10);
			if (proj.SpecialType == PST_PURPLE_FIREBALL)
				zombie.AddPoison(1);
		}
	};
}
