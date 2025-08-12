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
	/// @brief 毒的层数
	INT_PROPERTY(PoisonStack, __get_PoS, __set_PoS, 0x140);
	/// @brief 最近受到伤害的来源植物 ID
	INT_PROPERTY(LastDamageSourceID, __get_LaDSID, __set_LaDSID, 0x148);
	
	/// @brief 获取该僵尸掉落的经验值
	/// @return 掉落的经验值
	int GetBountyXP();

	/// @brief 根据盾的类型设置相应动画轨道的绘制分组。
	void AttachShield();
};