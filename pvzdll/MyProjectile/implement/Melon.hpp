#pragma once
#include "../ProjectileAbility.hpp"
#include <cmath>

namespace ProjectileAbility
{
	class Melon : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			if (damage_type == PVZEvent::ProjDmgType::DAMAGE_SPLASH_PRIMARY)
				return ori_dmg + proj.SpecialStack * 20;
			else if (damage_type == PVZEvent::ProjDmgType::DAMAGE_SPLASH_SECONDARY)
				return ori_dmg + proj.SpecialStack * 10;//原CT数值显然不合理，不过暂时不管
			return -1;
		}
		int GetImage(MyProjectile proj)
		{
			switch (proj.SourceType)
			{
			case SeedType::DarkShroom:
				return PVZ::Memory::ReadMemory<int>(0x6FF154);
			case SeedType::Mangosteen:
				return PVZ::Memory::ReadMemory<int>(0x6FF158);
			default:
				return -1;
			}
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			return original_val*sqrtf(1.0f+proj.SpecialStack*0.25f);
		}
	};
}
