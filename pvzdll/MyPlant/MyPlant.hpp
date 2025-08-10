#pragma once
#include "../framework.h"

class MyPlant : public PVZ::Plant
{
public:
	MyPlant(int idoraddress) : PVZ::Plant(idoraddress) {};
	MyPlant(const PVZ::Plant& plant) : PVZ::Plant(plant.GetBaseAddress()) {};

	/// @brief 生命恢复计时器
	INT_PROPERTY(HealCounter, __get_HeC, __set_HeC, 0x0D8);
	/// @brief 路灯花复活植物类型
	T_PROPERTY(SeedType::SeedType, RespawnType, __get_ReT, __set_ReT, 0x0E0);
	/// @brief 减速效果倒计时
	INT_PROPERTY(ChillCountdown, __get_ChC, __set_ChC, 0x104);
	/// @brief 当前等级
	INT_PROPERTY(Level, __get_Level, __set_Level, 0x118);

	/// @brief 治疗植物
	/// @note 不会超出生命值上限。
	/// @param val 治疗量
	void Heal(int val);

	/// @brief 最大等级
	static const int MAX_LEVEL = 5;

	/// @brief 根据识别 ID 获取对应植物。
	/// @param id 识别 ID
	/// @return ID 对应的植物。
	MyPlant GetByID(int id);
};