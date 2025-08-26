#pragma once
#include "MyProjectile.hpp"
#include "../MyPlant/MyPlant.hpp"
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
		/// @brief 子弹对僵尸造成伤害前，先结算的函数
		/// @note 触发时机比标记伤害来源晚。
		/// @param proj 子弹
		/// @param zombie 受伤的僵尸
		/// @param damage_type 伤害类型
		/// @param subtarget_count 溅射伤害溅射目标数
		/// @param ori_dmg 子弹即将造成的伤害。数值与 OverrideDamage() 的返回值相同。
		virtual void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return;
		}
		/// @brief 重载子弹图片的函数
		/// @param proj 子弹
		/// @return 子弹图片基址。若返回为负数，表示不重载。
		virtual int GetImage(MyProjectile proj)
		{
			return -1;
		}
		/// @brief 重载子弹图片尺寸的函数
		/// @param proj 子弹
		/// @param original_val 原始大小
		/// @return 子弹图片基址。若返回为负数，表示不重载。
		virtual float GetImageSize(MyProjectile proj, float original_val)
		{
			return original_val;
		}
	};
	typedef BaseProjectile* ProjectilePTR;
	ProjectilePTR GetAbility(ProjectileType::ProjectileType type);
}
