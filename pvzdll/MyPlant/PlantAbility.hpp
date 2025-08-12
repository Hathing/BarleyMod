#pragma once
#include "MyPlant.hpp"
#include "../MyZombie/MyZombie.hpp"

namespace PlantAbility
{
	class BasePlant
	{
	public:
		/// @brief 结算一次植物的技能更新
		/// @param plant 植物
		/// @return 是否结算原版的技能。
		/// @retval false 不结算独有技能，但仍然会结算一次性植物和射手攻击的技能效果。
		virtual bool TickAbility(MyPlant plant)
		{
			return true;
		}
		/// @brief 重载植物的攻击范围
		/// @param plant 植物
		/// @param secondary 是否使用副武器（裂荚射手左向、仙人掌对空等） 
		/// @param rect 存放重载后植物的攻击范围
		/// @return 若使用 rect 作为攻击范围，返回 true，否则返回 false 以使用原版默认的攻击范围
		virtual bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			return false;
		}
		/// @brief 植物更新独特外观（如坚果、高坚果）时，执行此函数。
		/// @param plant 植物
		/// @return 是否结算原版更新独特外观的过程。
		virtual bool onAnimate(MyPlant plant)
		{
			return true;
		}
		/// @brief 植物完成原版的初始化过程后，执行此函数。
		/// @param plant 植物
		virtual void onCreated(MyPlant plant)
		{
			return;
		}
		/// @brief 植物击杀僵尸时，先执行此函数。
		/// @param plant 植物
		/// @param zombie 僵尸
		virtual void onKill(MyPlant plant, MyZombie zombie)
		{
			return;
		}
	};
	typedef BasePlant* PlantPTR;
	PlantPTR GetPrototype(SeedType::SeedType type);
}
