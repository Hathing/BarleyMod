#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Kernel : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 20;
		}
		virtual int GetImage(MyProjectile proj)
		{
			if (proj.SourceType == SeedType::Mangosteen)
				return PVZ::Memory::ReadMemory<int>(0x6FF15C);
			return -1;
		}
	};
}
