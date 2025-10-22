#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Fireball : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			switch (damage_type)
			{
			case PVZEvent::DAMAGE_SINGULAR:
				return 20;
			case PVZEvent::DAMAGE_SPLASH_PRIMARY:
				return 20;
			case PVZEvent::DAMAGE_SPLASH_SECONDARY:
				return 5;
			default:
				break;
			}
			return ori_dmg;
		}
		void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			switch (proj.SpecialType)
			{
			case PST_ORANGE_FIREBALL:
				zombie.AddFlame(40);
				break;
			case PST_RED_FIREBALL:
				zombie.AddFlame(60);
				break;
			case PST_BLUE_FIREBALL:
				zombie.AddFrost(10);
				break;
			case PST_PURPLE_FIREBALL:
				zombie.AddPoison(20);
				break;
			default:
				zombie.AddFlame(20);
				break;
			}
		}
	};
}
