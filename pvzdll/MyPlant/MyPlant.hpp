#pragma once
#include "../framework.h"
#include "../Const.hpp"
#include "../MyProjectile/MyProjectile.hpp"

class MyPlant : public PVZ::Plant
{
public:
	MyPlant(int idoraddress) : PVZ::Plant(idoraddress) {};
	MyPlant(const PVZ::Plant& plant) : PVZ::Plant(plant.GetBaseAddress()) {};
	
	/// @brief 是否由大麦生成
	T_PROPERTY(mybool, FromBarley, __get_FrB, __set_FrB, 0x064);
	/// @brief 第一个与该植物相关的场地物件的 ID
	INT_PROPERTY(RelatedGriditemID1, __get_ReGID1, __set_ReGID1, 0x068);
	/// @brief 第二个与该植物相关的场地物件的 ID
	INT_PROPERTY(RelatedGriditemID2, __get_ReGID2, __set_ReGID2, 0x06C);
	/// @brief 被啃50cs倒计时
	INT_PROPERTY(EatenCounter, __get_EatenCounter, __set_EatenCounter, 0x0B4);
	/// @brief 发光倒计时
	INT_PROPERTY(LightCounter, __get_LightCounter, __set_LightCounter, 0x0B8);
	/// @brief 闪光倒计时
	INT_PROPERTY(FlashCounter, __get_FlashCounter, __set_FlashCounter, 0x0BC);
	/// @brief 技能计时器
	INT_PROPERTY(AnotherCounter, __get_AnC, __set_AnC, 0x0DC);
	/// @brief 磁力菇吸取物品的vx
	T_PROPERTY(float, MagnetItemXSpeed, __get_MagnetItemXSpeed, __set_MagnetItemXSpeed, 0xE0);
	/// @brief 磁力菇吸取物品的vy
	T_PROPERTY(float, MagnetItemYSpeed, __get_MagnetItemYSpeed, __set_MagnetItemYSpeed, 0xE4);
	/// @brief 磁力菇目标僵尸
	INT_PROPERTY(MagnetTarget, __get_MagnetTarget, __set_MagnetTarget, 0xE8);
	/// @brief 磁力菇当前状态，0表示不攻击，1表示攻击
	T_PROPERTY(byte, MagnetState, __get_MagnetState, __set_MagnetState, 0xEC);
	/// @brief 狙击豌豆的目标
	/// @deprecated
	INT_PROPERTY(PeashooterTarget, __get_PeashooterTarget, __set_PeashooterTarget, 0x0E0);
	/// @brief 机枪和裂荚和三线等植物开大波次，0则表示不开大
	T_PROPERTY(byte, UltraCount, __get_UltraCount, __set_UltraCount, 0xEC);
	/// @brief 植物每次重置+58后，在下一次重置+58之前FindTarget的次数，用于处理多连发植物开大概率的判断，目前只有裂荚在用
	T_PROPERTY(byte, FindTargetCount, __get_FindTargetCount, __set_FindTargetCount, 0xED);
	/// @brief 路灯花复活植物类型
	T_PROPERTY(SeedType::SeedType, RespawnType, __get_ReT, __set_ReT, 0x0E0);
	/// @brief 第一个与该植物相关的植物的 ID
	INT_PROPERTY(RelatedPlantID1, __get_RePID1, __set_RePID1, 0x0E0);
	/// @brief 第二个与该植物相关的植物的 ID
	INT_PROPERTY(RelatedPlantID2, __get_RePID2, __set_RePID2, 0x0E4);
	/// @brief 第三个与该植物相关的植物的 ID
	INT_PROPERTY(RelatedPlantID3, __get_RePID3, __set_RePID3, 0x0E8);
	/// @brief 第四个与该植物相关的植物的 ID
	INT_PROPERTY(RelatedPlantID4, __get_RePID4, __set_RePID4, 0x0EC);
	/// @brief 冰瓜投掷标记，0=正常，1=小冰瓜，2=大冰瓜
	T_PROPERTY(byte, WinterMelonCastState, __get_WinterMelonCastState, __set_WinterMelonCastState, 0x0E0);
	/// @brief 冰瓜散射计数，0=正常，>0表示单轮散射个数
	INT_PROPERTY(WinterMelonScatterCount, __get_WinterMelonScatterNum, __set_WinterMelonScatterNum, 0x0E4);
	/// @brief 水草对抓取目标施加效果的倒计时
	INT_PROPERTY(TangleKelpDoEffectCountdown, __get_TangleKelpDoEffectCountdown, __set_TangleKelpDoEffectCountdown, 0x0E0);
	/// @brief 金盏花技能类型，0=无技能，1=水壶，2=钉耙，3=杀虫剂，4=肥料
	T_PROPERTY(byte, MarigoldItemType, __get_MarigoldItemType, __set_MarigoldItemType, 0x0E0);
	/// @brief 金盏花技能目标ID
	INT_PROPERTY(MarigoldTargetID, __get_MarigoldTargetID, __set_MarigoldTargetID, 0x0E4);
	/// @brief 双发下次发射的子弹种类
	INT_PROPERTY(RepeaterNextProjType, __get_RepeaterNextProjType, __set_RepeaterNextProjType, 0x0E0);
	/// @brief 双发记录自身发射过的子弹种类
	INT_PROPERTY(ReapeaterRecord, __get_ReapeaterRecord, __set_ReapeaterRecord, 0x0E4); 
	/// @brief 生命恢复计时器
	INT_PROPERTY(HealCounter, __get_HeC, __set_HeC, 0x0F0);
	/// @brief 当前体型值
	T_PROPERTY(float, BodySize, __get_BodySize, __BodySize, 0xF4);
	/// @brief 体型变化动画倒计时
	INT_PROPERTY(BodySizeCountdown, __get_BodySizeCountdown, __set_BodySizeCountdown, 0xFC);
	/// @brief 减速效果倒计时
	INT_PROPERTY(ChillCountdown, __get_ChC, __set_ChC, 0x104);
	/// @brief 胆小菇击杀次数
	INT_PROPERTY(KillCount, __get_KiC, __set_KiC, 0x108);
	/// @brief 是否启用彩蛋皮
	T_PROPERTY(mybool, EasterSkin, __get_EaS, __set_EaS, 0x10C);
	/// @brief 海蘑菇子编号
	T_PROPERTY(byte, SubIndex, __get_SuI, __set_SuI, 0x10D);
	/// @brief 植物的所有者的 ID
	INT_PROPERTY(OwnerID, __get_SpO, __set_SpO, 0x110);
	/// @brief 当前经验值
	INT_PROPERTY(Experience, __get_XP, __set_XP, 0x114);
	/// @brief 当前等级
	INT_PROPERTY(Level, __get_Level, __set_Level, 0x118);
	/// @brief 护盾点数
	INT_PROPERTY(DamageAbsorption, __get_DaA, __set_DaA, 0x11C);
	/// @brief 大麦及其派生植物变身倒计时
	INT_PROPERTY(BarleyCounter, __get_BaC, __set_BaC, 0x120);
	/// @brief 肥料倒计时
	INT_PROPERTY(FertilizedCounter, __get_FertilizedCounter, __set_FertilizedCounter, 0x124);
	/// @brief 植物血条显示倒计时
	INT_PROPERTY(HpDisplayCounter, __get_HealthDisplayCounter, __set_HealthDisplayCounter, 0x130);
	/// @brief 是否在 Board 上
	T_PROPERTY(mybool, OnBoard, __get_OnB, __set_OnB, 0x144);
	/// @brief 投篮车索敌标记
	T_PROPERTY(byte, CatapultTargetSkip, __get_CatapultTargetSkip, __set_CatapultTargetSkip, 0x146);

	/// @brief 获取植物的所有者。
	/// @return 植物的所有者
	MyPlant GetOwner();
	/// @brief 设置植物的所有者
	/// @param owner 设置后的所有者 
	void SetOwner(MyPlant owner);
	/// @brief 设置植物的最大生命值。
	/// @note 植物的生命值会发生相同改变量的变化。
	/// @param val 调整后的最大生命值
	void SetMaxHealth(int val);

	/// @brief 判断该植物是否为工具植物
	/// @return 是否为工具植物
	bool IsToolPlant();

	/// @brief 判断该植物是否为主植物
	/// @return 是否为主植物
	bool IsPrime();
	/// @brief 判断该植物是否可以获得经验值
	/// @return 是否可以获得经验值
	bool IsXPRecipient();

	/// @brief 判断该植物是否可升级
	/// @return 是否可升级
	bool CanUpgrade();
	/// @brief 升级的基本事件,如创建升级动画
	void Upgrade();

	/// @brief 治疗植物
	/// @note 不会超出生命值上限。
	/// @param val 治疗量
	/// @return 溢出的治疗量
	int Heal(int val);
	/// @brief 检查是否符合升级条件。若符合，则立刻升级。
	/// @return 是否升级
	bool CheckUpgrade();
	/// @brief 获得经验值
	/// @param val 获得的经验数值
	/// @param kill_credit 是否为击杀奖励
	void AddExperience(int val, bool kill_credit = false);
	/// @brief 启用彩蛋皮
	void EnableEasterSkin();
	/// @brief 植物索敌并准备开火。只有除了三线和杨桃的攻击型植物才应当使用这个函数
	/// @param row 索敌行
	/// @param PlantWeapon 大多数植物=0，裂荚后射、仙人掌在地面射、玉米黄油等，则=1
	/// @return 返回一个bool，表示植物是否成功索敌
	bool FindTargetAndFire(int row, int PlantWeapon);
	/// @brief 植物寻找敌人。
	/// @param PlantWeapon 大多数植物=0，裂荚后射、仙人掌在地面射、玉米黄油等，则=1
	/// @return 僵尸ID，仅仅用于开火的参数
	int FindTargetZombie(int PlantWeapon);
	/// @brief 植物开火。PVZClass的Shoot()不知道为什么用了会崩溃，这个函数直接调用原版函数466e00
	/// @param PlantWeapon 大多数植物=0，裂荚后射、仙人掌在地面射、玉米黄油等，则=1
	/// @param targetid 目标僵尸
	void Fire(int PlantWeapon,int targetid);
	/// @brief 三线射手调用此函数
	void LaunchThreepeater();
	/// @brief 所有植物通用的初始化标记一个子弹的属性
	/// @param proj 子弹
	void InitAddProjectile(MyProjectile proj);

	/// @brief 植物受到伤害，封装了各种事件。
	/// @param source 伤害来源
	/// @param source_type 伤害来源的类型
	/// @param damage 伤害值
	/// @return 实际受到伤害值(伤害<0则失败)
	/// @deprecated
	/// int TakeDamage(PVZ::BaseClass source, GameObjectType::GameObjectType source_type, int damage);
	
	/// @brief 最大等级
	static const int MAX_LEVEL = 5;

	/// @brief 根据识别 ID 获取对应植物。
	/// @param id 识别 ID
	/// @return ID 对应的植物。
	static MyPlant GetByID(int id);
};

inline void StartBlend(int blendtime, PVZ::Animation anim);