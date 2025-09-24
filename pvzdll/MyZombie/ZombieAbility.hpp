#pragma once
#include "../MyClasses.hpp"

namespace ZombieAbility
{
	class BaseZombie
	{
	public:
		/// @brief 僵尸完成原版的初始化过程后，执行此函数。
		/// @param zombie 僵尸
		virtual void onCreated(MyZombie zombie)
		{
			return;
		}
		/// @brief 僵尸生成掉落物时，先调用该函数。
		/// @param zombie 僵尸
		virtual void onKilled(MyZombie zombie)
		{
			return;
		}
		/// @brief 重载僵尸的绘制位置
		/// @param zombie 僵尸
		/// @param draw_pos 僵尸的绘制位置
		/// @return 是否执行原版对 BodyY 和 ClipRect 的设定
		virtual bool OverrideDrawPos(MyZombie zombie, PVZ::ZombieDrawPosition* draw_pos)
		{
			return false;
		}
	};
	typedef BaseZombie* ZombiePTR;
	ZombiePTR GetAbility(ZombieType::ZombieType type);
}
