#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Kernel : public BaseProjectile
	{
	public:
		virtual int GetImage(MyProjectile proj)
		{
			if (proj.SourceType == SeedType::Mangosteen)
				return PVZ::Memory::ReadMemory<int>(0x6FF15C);
			return -1;
		}
	};
}
