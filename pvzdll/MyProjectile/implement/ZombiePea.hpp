#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class ZombiePea : public BaseProjectile
	{
	public:
		int GetImage(MyProjectile proj)
		{
			if (proj.SpecialFlags == 1)
				//??Ã»¿´¶®
				return PVZ::Memory::ReadMemory<int>(0x6a7408);
			return -1;
		}
	};
}
