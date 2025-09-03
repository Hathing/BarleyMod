#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Cabbage : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			static const int dmg[6] = {40, 60, 60, 80, 80, 80};
			return dmg[min(MyPlant::MAX_LEVEL, proj.SourceLevel)];
		}
		int GetImage(MyProjectile proj)
		{
			return proj.SourceLevel == MyPlant::MAX_LEVEL ? PVZ::Memory::ReadMemory<int>(0x6FF040) : -1;
		}
	};
}
