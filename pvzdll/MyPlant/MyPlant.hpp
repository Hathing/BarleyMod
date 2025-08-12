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
	/// @brief 胆小菇击杀次数
	INT_PROPERTY(KillCount, __get_KiC, __set_KiC, 0x108);
	/// @brief 是否启用彩蛋皮
	T_PROPERTY(mybool, EasterSkin, __get_EaS, __set_EaS, 0x10C);
	/// @brief 植物的所有者的 ID
	INT_PROPERTY(OwnerID, __get_SpO, __set_SpO, 0x110);
	/// @brief 当前等级
	INT_PROPERTY(Level, __get_Level, __set_Level, 0x118);

	/// @brief 获取植物的所有者。
	/// @return 植物的所有者
	MyPlant GetOwner();
	/// @brief 设置植物的所有者
	/// @param owner 设置后的所有者 
	void SetOwner(MyPlant owner);

	/// @brief 判断该植物是否为工具植物
	/// @return 是否为工具植物
	bool IsToolPlant();

	/// @brief 治疗植物
	/// @note 不会超出生命值上限。
	/// @param val 治疗量
	void Heal(int val);
	/// @brief 获得经验值
	/// @param val 获得的经验数值
	/// @param kill_credit 是否为击杀奖励
	void AddExperience(int val, bool kill_credit = false);
	/// @brief 启用彩蛋皮
	void EnableEasterSkin();

	/// @brief 最大等级
	static const int MAX_LEVEL = 5;

	/// @brief 根据识别 ID 获取对应植物。
	/// @param id 识别 ID
	/// @return ID 对应的植物。
	static MyPlant GetByID(int id);
};