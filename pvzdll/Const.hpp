#pragma once
#include "framework.h"

namespace SeedType
{
	typedef SeedType Type2;
	const Type2 Barley = (Type2)1;
	const Type2 DiamondShroom = (Type2)0x10;
	const Type2 RestingBarley = (Type2)0x29;
	const Type2	Endoflame = (Type2)51;
	const Type2	SpringEquinoxVine = (Type2)0x4C;
	const Type2 DarkShroom = (Type2)0x4D;
	const Type2 Mangosteen = (Type2)0x4E;
	const Type2 ThymeWarp = (Type2)0x4F;
}

namespace ZombieType
{
	typedef ZombieType Type2;
	constexpr Type2 RandomZombie = ConeheadZombie;
}

namespace ProjectileType
{
	typedef ProjectileType Type2;
	const Type2 GoldSpike = (Type2)15;
	const Type2 Diamond = (Type2)18;
	const Type2 GoldMelon = (Type2)22;
}

namespace GriditemType
{
	typedef GriditemType Type2;
	constexpr Type2 SpringEquinoxFireGrid = (Type2)14;
	constexpr Type2 SpringEquinoxIceGrid = (Type2)15;
}

namespace PVZEvent
{
	typedef PlantDamageType Type2;
	constexpr Type2 PLANTDAMAGETYPE_CUSTOM = (Type2)16;
	constexpr Type2 PLANTDAMAGETYPE_PROJECTILE = (Type2)10;
}

namespace PlantState
{
	typedef PlantState Type2;
	constexpr Type2 HYPNOSHROOM_SUMMONING = (Type2)50;
}

namespace MotionType
{
	typedef MotionType Type2;
	constexpr Type2 Piercing = (Type2)10;
}

constexpr int WAVE_ELITE_MASK = 5000;
constexpr int WAVE_ELITE1 = WAVE_ELITE_MASK + 1;
constexpr int WAVE_ELITE2 = WAVE_ELITE_MASK + 2;