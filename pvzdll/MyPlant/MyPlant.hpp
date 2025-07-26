#pragma once
#include "../framework.h"

class MyPlant : public PVZ::Plant
{
public:
	MyPlant(int idoraddress) : PVZ::Plant(idoraddress) {};
	MyPlant(const PVZ::Plant& plant) : PVZ::Plant(plant.GetBaseAddress()) {};

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