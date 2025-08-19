#pragma once
#include "MyProjectile.hpp"

namespace ProjectileAbility
{
	class BaseProjectile
	{
	};
	typedef BaseProjectile* ProjectilePTR;
	ProjectilePTR GetAbility(ProjectileType::ProjectileType type);
}
