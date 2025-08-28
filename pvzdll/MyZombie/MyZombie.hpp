#pragma once
#include "../framework.h"

class MyZombie : public PVZ::Zombie
{
public:
	MyZombie(int idoraddress) : PVZ::Zombie(idoraddress) {};
	MyZombie(const PVZ::Zombie& zombie) : PVZ::Zombie(zombie.GetBaseAddress()) {};

	/// @brief 是否掉落过掉落物
	T_PROPERTY(byte, DroppedLoot, __get_DrL, __set_DrL, 0x70);
	T_PROPERTY(byte, Unknown, __get_Un, __set_Un, 0x71);
	/// @brief 精英类别
	T_PROPERTY(byte, EliteType, __get_ElT, __set_ElT, 0x72);
	/// @brief 僵尸被多投索敌过的标记
	T_PROPERTY(byte, PultSkip, __get_PultSkip, __set_PultSkip, 0x106);
	/// @brief 毒的层数
	INT_PROPERTY(PoisonStack, __get_PoS, __set_PoS, 0x140);
	/// @brief 最近受到伤害的来源植物 ID
	INT_PROPERTY(LastDamageSourceID, __get_LaDSID, __set_LaDSID, 0x148);
	
	/// @brief 获取该僵尸掉落的经验值
	/// @return 掉落的经验值
	int GetBountyXP();

	/// @brief 根据盾的类型设置相应动画轨道的绘制分组。
	void AttachShield();

	/// @brief 僵尸飞到屏幕右侧外
	/// @param factor 被击飞后水平速度的倍数，取负数让僵尸反向飞出屏幕
	void FlyAway(float factor);
};