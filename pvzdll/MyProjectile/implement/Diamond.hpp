#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Diamond : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			static const int dmg[6] = { 40, 40, 45, 45, 50, 50 };

			auto owner = MyPlant::GetByID(proj.ParentID);
			int lvl = 0;
			if (owner.isValid())
				lvl = min(MyPlant::MAX_LEVEL, owner.Level); \

			return dmg[lvl];
		}
		virtual int GetImage(MyProjectile proj)
		{
			return PVZ::Memory::ReadMemory<int>(0x6FF010);
		}
	};
}
