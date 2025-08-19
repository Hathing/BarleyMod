#include "MyProjectile.hpp"
#include "implement/index.hpp"

namespace ProjectileAbility
{
	ProjectilePTR pt_factory[] =
	{
		new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(),
		new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(),
		new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(),
		new GoldSpike(),	  new BaseProjectile(), new BaseProjectile(), new BaseProjectile(), new BaseProjectile(),
	};
}

ProjectileAbility::ProjectilePTR ProjectileAbility::GetAbility(ProjectileType::ProjectileType type)
{
	return ProjectileAbility::pt_factory[type];
}
