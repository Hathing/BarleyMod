#pragma once
#include "../framework.h"

class MyZombie : public PVZ::Zombie
{
public:
	MyZombie(int idoraddress) : PVZ::Zombie(idoraddress) {};
	MyZombie(const PVZ::Zombie& zombie) : PVZ::Zombie(zombie.GetBaseAddress()) {};

	/// @brief 默认=0，=1时，如果+64=7，击飞；如果+64=0，击退。
	T_PROPERTY(byte, IsLaunched, __get_IsLaunched, __set_IsLaunched, 0x52);
	/// @brief 生成的波数
	INT_PROPERTY(FromWave, __get_FrW, __set_FrW, 0x6C);
	/// @brief 是否掉落过掉落物
	T_PROPERTY(byte, DroppedLoot, __get_DrL, __set_DrL, 0x70);
	/// @brief 僵尸的特殊标记，=1表示被精英小丑标记爆炸，=9表示虚影
	/// @deprecated 精英小丑与虚影拟定废除或者重做，不再使用该指针，该属性仅用于协助阅读CT！
	T_PROPERTY(byte, SpecialFlag, __get_SpecialFlag, __set_SpecialFlag, 0x71);
	/// @brief 精英类别
	/// @deprecated 请使用 WAVE_ELITE 系列常量和 FromWave 替代之。
	T_PROPERTY(byte, EliteType, __get_ElT, __set_ElT, 0x72);
	/// @brief 是否受钢地刺影响，啃食伤害减半
	T_PROPERTY(BOOLEAN, IsWeak, __get_IsW, __set_IsW, 0x73);
	/// @brief 燃烬层数过去使用的地址
	/// @deprecated 已转移至FlameStack属性，不再使用该指针，该属性仅用于协助阅读CT！
	INT_PROPERTY(DeprecatedFlameStack, __get_DeprecatedFlameStack, __set_DeprecatedFlameStack, 0x80);
	/// @brief 召唤该僵尸的植物的 ID
	/// @note 由于适配问题，这里暂时直接存基址
	INT_PROPERTY(SourceID, __get_SoID, __set_SoID, 0x0E8);
	/// @brief 僵尸被多投索敌过的标记
	T_PROPERTY(byte, PultSkip, __get_PultSkip, __set_PultSkip, 0x105);
	/// @brief 是否被冰封
	T_PROPERTY(mybool, IceBounded, __get_IcB, __set_IcB, 0x105);
	/// @brief 魅惑菇鬼火标记
	T_PROPERTY(byte, GhostFlameMark, __get_GhFM, __set_GhFM, 0x106);
	/// @brief 被魅惑菇召唤时，魅惑菇的等级
	T_PROPERTY(byte, SourceLevel, __get_SoL, __set_SoL, 0x107);
	/// @brief 僵尸受攻击的动画附件的ID
	INT_PROPERTY(HitReanimID, __get_HitReanimID, __set_HitReanimID, 0x110);
	/// @brief 僵尸的掉落速度，原版中只有小鬼和水族馆僵尸使用该指针；负数表示下落，正数表示上升
	T_PROPERTY(float, FallSpeed, __get_FallSpeed, __set_FallSpeed, 0x120);
	/// @brief 巨人变种类型
	/// @deprecated 已转移至VariantType属性，不再使用该指针，该属性仅用于协助阅读CT！
	INT_PROPERTY(DeprecatedGargantaurType, __get_GaT, __set_GaT, 0x128);
	/// @brief 金色标记
	INT_PROPERTY(GoldMark, __get_GoM, __set_GoM, 0x12C);
	/// @brief 伤害免疫持续时间
	INT_PROPERTY(InvulnerableDuration, __get_InD, __set_InD, 0x12C);
	/// @brief 寒意持续时间
	INT_PROPERTY(ChillDuration, __get_ChD, __set_ChD, 0x130);
	/// @brief 是否已被嘲讽
	T_PROPERTY(byte, Taunted, __get_Ta, __set_Ta, 0x134);
	/// @brief 僵尸血量点数标记，非盲盒开出的僵尸该值为0
	T_PROPERTY(byte, HpPoint, __get_HpPoint, __set_HpPoint, 0x138);
	/// @brief 僵尸反向标记，1=反向回头，2=反向出屏幕
	T_PROPERTY(byte, IsWalkingBackwards, __get_IsWalkingBackwards, __set_IsWalkingBackwards, 0x13C);
	/// @brief 僵尸寒意层数，上限30层，每层减速2%
	T_PROPERTY(byte, FrostStack, __get_FrostStack, __set_FrostStack, 0x13D);
	/// @brief 僵尸显示颜色标记，0为不显示，1为毒，2为冰，3为火
	T_PROPERTY(byte, ColorFlag, __get_ColorFlag, __set_ColorFlag, 0x13E);
	/// @brief 非精英僵尸变种类型
	T_PROPERTY(ZombieVariantType::ZombieVariantType, VariantType, __get_VariantType, __set_VariantType, 0x13F);
	/// @brief 毒的层数
	INT_PROPERTY(PoisonStack, __get_PoS, __set_PoS, 0x140);
	/// @brief 燃烬层数
	INT_PROPERTY(FlameStack, __get_FlameStack, __set_FlameStack, 0x14C);
	/// @brief 最近受到伤害的来源植物 ID
	INT_PROPERTY(LastDamageSourceID, __get_LaDSID, __set_LaDSID, 0x148);
	
	/// @brief 读报的怒气层数，掉报纸期间每受到一次伤害就增加一层
	INT_PROPERTY(NewspaperAngerStack, __get_NewspaperAngerStack, __set_NewspaperAngerStack, 0xF0);
	/// @brief 矿工上次刨根的列数
	INT_PROPERTY(DiggerLastDigRootColumn, __get_DiggerLastDigRootColumn, __set_DiggerLastDigRootColumnn, 0xF0);
	/// @brief 舞王和雪橇车相关联僵尸编号数组属性
	INT_ARRAY_PROPERTY(GetRelatedZombieID, SetRelatedZombieID, 0xF0);

	/// @brief 获取该僵尸掉落的经验值
	/// @return 掉落的经验值
	int GetBountyXP();

	/// @brief 根据盾的类型设置相应动画轨道的绘制分组。
	void AttachShield();

	/// @brief 僵尸飞到屏幕右侧外
	/// @param factor 被击飞后水平速度的因子，取负数让僵尸反向飞出屏幕，但是这个参数理论来说没啥用？
	void FlyAway(float factor);
	/// @brief 寻找可攻击的僵尸目标，封装的原版函数
	/// @return 可攻击的僵尸
	MyZombie FindZombieTarget();
	/// @brief 寻找可攻击的植物目标，封装的原版函数
	/// @return 可攻击的植物
	PVZ::Plant FindPlantTarget(int attacktype);

	/// @brief 使僵尸停止啃咬
	void StopEating();
	/// @brief 重置僵尸的速度
	void PickRandomSpeed();
	/// @brief 让僵尸从土里或者泳池里冒出来，通常在生成后调用该函数
	void RiseFromGrave(int row,int column);

	/// @brief 增加寒意值
	/// @note 寒意的上限层数只有30层，使用byte储存，请注意溢出问题！
	/// @param num 增加的数值
	void AddFrost(int num);
	/// @brief 增加剧毒值
	/// @param num 增加的数值
	void AddPoison(int num);
	/// @brief 增加燃烬值
	/// @param num 增加的数值
	void AddFlame(int num);

	/// @brief 僵尸被击飞
	/// @note 使用前请先在函数外处理对气球等空中僵尸的判断
	void Launch(float xspeed,float yspeed,float startheight = 1.0f);
	/// @brief 僵尸被击退
	/// @note 使用前请先在函数外处理对气球等空中僵尸的判断
	void KnockBack(float xspeed);

	/// @brief 判断僵尸是否被水草缠住
	/// @return 是则返回True
	bool IsTangleKelpTarget();
	/// @brief 矿工失去镐子
	void DiggerLoseAxe();
	/// @brief 投篮车尝试寻找目标植物
	/// @return 目标植物的Address，没有找到目标则为0。
	int FindCatapultTarget();
	/// @brief 投篮车对目标植物开火
	/// @param 目标植物的Address，0表示没有找到目标。
	void ZombieCatapultFire(int targetaddr);
	/// @brief 调用原版函数，返回雪橇小队中某个僵尸的编号
	/// @return 该僵尸在小队中的编号，0为队长，-1表示不在小队中。
	int GetBobsledPosition();
	/// @brief 盲盒僵尸获取生命分数
	/// @return 生命分数的分子
	int GenHpPoint();
};