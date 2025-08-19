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
		void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			if (zombie.Type != ZombieType::Gargantuar && zombie.Type != ZombieType::Gigagargantuar)
				zombie.X += 10;
		}
	};
	typedef BaseProjectile* ProjectilePTR;
	ProjectilePTR GetAbility(ProjectileType::ProjectileType type);
}
