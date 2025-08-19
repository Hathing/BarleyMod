#pragma once
#include "MyProjectile.hpp"
#include "../MyZombie/MyZombie.hpp"

namespace ProjectileAbility
{
	class BaseProjectile
	{
	public:
		/// @brief 重载子弹对僵尸伤害的函数
		/// @param proj 子弹
		/// @param zombie 受伤的僵尸
		/// @param damage_type 伤害类型
		/// @param subtarget_count 溅射伤害溅射目标数
		/// @param ori_dmg 溅射伤害数值
		/// @return 若为负数，则不重载伤害，否则为重载的伤害数值
		virtual int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return -1;
		}
	};
	typedef BaseProjectile* ProjectilePTR;
	ProjectilePTR GetAbility(ProjectileType::ProjectileType type);
}
