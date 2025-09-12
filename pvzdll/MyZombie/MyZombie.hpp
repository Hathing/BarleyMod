#pragma once
#include "../framework.h"

class MyZombie : public PVZ::Zombie
{
public:
	MyZombie(int idoraddress) : PVZ::Zombie(idoraddress) {};
	MyZombie(const PVZ::Zombie& zombie) : PVZ::Zombie(zombie.GetBaseAddress()) {};

	/// @brief 生成的波数
	INT_PROPERTY(FromWave, __get_FrW, __set_FrW, 0x6C);
	/// @brief 是否掉落过掉落物
	T_PROPERTY(byte, DroppedLoot, __get_DrL, __set_DrL, 0x70);
	T_PROPERTY(byte, Unknown, __get_Un, __set_Un, 0x71);
	/// @brief 精英类别
	T_PROPERTY(byte, EliteType, __get_ElT, __set_ElT, 0x72);
	/// @brief 召唤该僵尸的植物的 ID
	/// @note 由于适配问题，这里暂时直接存基址
	INT_PROPERTY(SourceID, __get_SoID, __set_SoID, 0x0E8);
	/// @brief 僵尸被多投索敌过的标记
	T_PROPERTY(byte, PultSkip, __get_PultSkip, __set_PultSkip, 0x106);
	/// @brief 魅惑菇鬼火标记
	T_PROPERTY(byte, GhostFlameMark, __get_GhFM, __set_GhFM, 0x106);
	/// @brief 被魅惑菇召唤时，魅惑菇的等级
	T_PROPERTY(byte, SourceLevel, __get_SoL, __set_SoL, 0x107);
	/// @brief 僵尸反向标记，1=反向回头，2=反向出屏幕
	T_PROPERTY(byte, IsWalkingBackwards, __get_IsWalkingBackwards, __set_IsWalkingBackwards, 0x13C);
	/// @brief 僵尸寒意层数，上限30层，每层减速2%
	T_PROPERTY(byte, FrostStack, __get_FrostStack, __set_FrostStack, 0x13D);
	/// @brief 僵尸显示颜色标记，0为不显示，1为毒，2为冰，3为火
	T_PROPERTY(byte, ColorFlag, __get_ColorFlag, __set_ColorFlag, 0x13E);
	/// @brief 僵尸血量点数标记，非盲盒开出的僵尸该值为0
	T_PROPERTY(byte, HpPoint, __get_HpPoint, __set_HpPoint, 0x13F);
	/// @brief 毒的层数
	INT_PROPERTY(PoisonStack, __get_PoS, __set_PoS, 0x140);
	/// @brief 燃烬层数
	INT_PROPERTY(FlameStack, __get_FlameStack, __set_FlameStack, 0x14C);
	/// @brief 最近受到伤害的来源植物 ID
	INT_PROPERTY(LastDamageSourceID, __get_LaDSID, __set_LaDSID, 0x148);
	
	/// @brief 获取该僵尸掉落的经验值
	/// @return 掉落的经验值
	int GetBountyXP();

	/// @brief 根据盾的类型设置相应动画轨道的绘制分组。
	void AttachShield();

	/// @brief 僵尸飞到屏幕右侧外
	/// @param factor 被击飞后水平速度的因子，取负数让僵尸反向飞出屏幕，但是这个参数理论来说没啥用？
	void FlyAway(float factor);
	/// @brief 寻找可攻击的僵尸目标，封装的原版函数
	/// return 可攻击的僵尸
	MyZombie FindZombieTarget();

	// @brief 增加寒意值
	// @note 寒意的上限层数只有30层，使用byte储存，请注意溢出问题！
	// @param num 增加的数值
	void AddFrost(int num);
	// @brief 增加剧毒值
	// @param num 增加的数值
	void AddPoison(int num);
	// @brief 增加燃烬值
	// @param num 增加的数值
	void AddFlame(int num);

};