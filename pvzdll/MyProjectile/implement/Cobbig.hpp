#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Cobbig : public BaseProjectile
	{
	public:
		int GetImage(MyProjectile proj)
		{
			return proj.SourceType == SeedType::Mangosteen ? PVZ::Memory::ReadMemory<int>(0x6FF158) : -1;
		}
	};
}
