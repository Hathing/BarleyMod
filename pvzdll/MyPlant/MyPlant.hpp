#pragma once
#include "../framework.h"
#include "../Const.hpp"
#include "../MyZombie/MyZombie.hpp"

class MyPlant : public PVZ::Plant
{
public:
	MyPlant(int idoraddress) : PVZ::Plant(idoraddress) {};
	MyPlant(const PVZ::Plant& plant) : PVZ::Plant(plant.GetBaseAddress()) {};

	/// @brief 是否由大麦生成
	T_PROPERTY(mybool, FromBarley, __get_FrB, __set_FrB, 0x064);
	/// @brief 被啃50cs倒计时
	T_PROPERTY(byte, EatenCounter, __get_EatenCounter, __set_EatenCounter, 0x0B4);
	/// @brief 技能计时器
	INT_PROPERTY(AnotherCounter, __get_AnC, __set_AnC, 0x0DC);
	/// @brief 狙击豌豆的目标
	INT_PROPERTY(PeashooterTarget, __get_PeashooterTarget, __set_PeashooterTarget, 0x0E0);
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
	/// @brief 生命恢复计时器
	INT_PROPERTY(HealCounter, __get_HeC, __set_HeC, 0x0F0);
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
	/// @brief 大麦及其派生植物变身倒计时
	INT_PROPERTY(BarleyCounter, __get_BaC, __set_BaC, 0x120);
	/// @brief 植物血条显示倒计时
	INT_PROPERTY(HpDisplayCounter, __get_HealthDisplayCounter, __set_HealthDisplayCounter, 0x130);
	/// @brief 是否在 Board 上
	T_PROPERTY(mybool, OnBoard, __get_OnB, __set_OnB, 0x144);

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
	void Heal(int val);
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
	/// @param PlantWeapon 大多数植物=0，裂荚后射、仙人掌在地面射、玉米黄油等，则=1
	/// @return 返回一个bool，表示植物是否成功索敌
	bool FindTargetAndFire(int PlantWeapon);
	/// @brief 植物寻找敌人。
	/// @param PlantWeapon 大多数植物=0，裂荚后射、仙人掌在地面射、玉米黄油等，则=1
	/// @return 僵尸ID，仅仅用于开火的参数
	int FindTargetZombie(int PlantWeapon);
	/// @brief 植物开火。PVZClass的Shoot()不知道为什么用了会崩溃，这个函数直接调用原版函数466e00
	/// @param PlantWeapon 大多数植物=0，裂荚后射、仙人掌在地面射、玉米黄油等，则=1
	/// @param targetid 目标僵尸
	void Fire(int PlantWeapon,int targetid);

	/// @brief 最大等级
	static const int MAX_LEVEL = 5;

	/// @brief 根据识别 ID 获取对应植物。
	/// @param id 识别 ID
	/// @return ID 对应的植物。
	static MyPlant GetByID(int id);
};