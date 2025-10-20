#pragma once
#include "../MyClasses.hpp"

namespace PlantAbility
{
	class BasePlant
	{
	public:
		/// @brief 升级时，修改植物的属性，例如攻速提高、血量提高
		/// @param plant 植物
		virtual void UpgradeModification(MyPlant plant)
		{
			return;
		}
		/// @brief 结算一次植物的技能更新
		/// @param plant 植物
		/// @return 是否结算原版的技能。
		/// @retval false 不结算独有技能，但仍然会结算一次性植物和射手攻击的技能效果。
		virtual bool TickAbility(MyPlant plant)
		{
			return true;
		}
		/// @brief 结算一次植物的被动技能更新
		/// @note 此函数的调用不受植物加速减速的影响。
		/// @param plant 植物
		virtual void TickPassive(MyPlant plant)
		{
			return;
		}
		/// @brief 结算一次植物的自回复，默认结算周期为100cs
		/// @note 此函数的调用不受植物加速减速的影响。
		/// @param plant 植物
		virtual void SelfHeal(MyPlant plant)
		{
			//大部分植物每秒回复1点HP
			plant.Heal(1);
			return;
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
		/// @brief 植物被移除时，先执行此函数。
		/// @note 不在 Board 上的植物也会调用此函数。
		/// @attention 这个函数只应该负责清理技能衍生物，不用于也不该用于结算亡语。
		/// @param plant 植物
		virtual void onDie(MyPlant plant)
		{
			return;
		}
		/// @brief 判断植物是否可以获得经验值
		/// @param plant 植物
		/// @return 是否可以获得经验值
		virtual bool IsXPRecipient(MyPlant plant)
		{
			if (plant.Squash)
				return false;
			if (plant.Type == SeedType::Seashroom && !plant.IsPrime())
				return false;
			return plant.Level < MyPlant::MAX_LEVEL || plant.IsToolPlant();
		}
		/// @brief 植物获得经验值时，结算此函数。
		/// @note 此时植物已经获得经验值
		/// @param plant 植物
		/// @param val 经验数值
		/// @param kill_credit 是否为击杀奖励 
		virtual void onGainXP(MyPlant plant, int val, bool kill_credit)
		{
			return;
		}
		/// @brief 植物升级时，结算此函数。
		/// @note 此时植物等级的数值已提升
		/// @param plant 植物
		virtual void onUpgrade(MyPlant plant)
		{
			return;
		}
		/// @brief 植物发射子弹事件。
		/// @param 依次为：触发事件的植物、生成的子弹、子弹目标僵尸的基址、PlantWeapon。
		/// @return 是否进行原版的初始化过程。
		virtual bool onAddProjectile(MyPlant plant, MyProjectile proj, MyZombie zombie, int PlantWeapon)
		{
			return true;
		}
		/// @brief 植物更新原版射击过程前，先执行此函数
		/// @param 触发事件的植物
		/// @return 是否进行原版的发射。
		virtual bool onUpdateShooter(MyPlant plant)
		{
			return true;
		}
		/// @brief 植物在+90>0时尝试开火的更新
		/// @param 触发事件的植物
		/// @return 是否进行原版的更新。如果要跳过原版更新，要注意手动减+90、重置动画等处理！
		virtual bool onUpdateShooting(MyPlant plant)
		{
			return true;
		}
		/// @brief 植物索敌僵尸事件
		/// @note 该事件只重载行差判定和类型判定
		/// @param 触发事件的植物，植物判定的僵尸，索敌行
		/// @return 若为负数，则按原版处理；若为 0，则为不可选中；若为正数，则为强制可选。
		virtual int onFindTargetRT(MyPlant plant, MyZombie zombie, int row)
		{
			return ThreeState::None;
		}
		/// @brief 植物开火生成子弹前的事件。
		/// @param plant 植物
		/// @param target 目标僵尸
		/// @param weapon_type 武器类型
		/// @return False则跳过子弹生成
		virtual bool onFire(MyPlant plant, MyZombie target, int weapon_type)
		{
			return true;
		}
		/// @brief 获取植物满级时启用彩蛋皮的概率的倒数
		/// @return 植物满级时启用彩蛋皮的概率的倒数。
		/// @retval 0 禁用彩蛋皮。
		virtual int GetEasterProbabilityPartition()
		{
			return 10;
		}
		virtual void onEnableEasterSkin(MyPlant plant)
		{
			return;
		}
		/// @brief 植物对僵尸直接造成伤害时，先执行此函数
		/// @param info 伤害信息
		virtual void OverwritePZDamage(PZDamageEvent* info)
		{
			return;
		}
		/// @brief 获取植物索敌标签
		/// @param plant 植物
		/// @param weapon_type 武器类型
		/// @return 植物索敌标签。若为负数，则改为使用原版数值。
		virtual int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return -1;
		}
		/// @brief 获取植物索敌优先级
		/// @param plant 植物
		/// @param zombie 目标僵尸
		/// @param original_priority 原始优先级
		/// @return 索敌优先级
		virtual int GetTargetZombiePriority(MyPlant plant, MyZombie zombie, int original_priority)
		{
			return original_priority;
		}
	};
	typedef BasePlant* PlantPTR;
	PlantPTR GetAbility(SeedType::SeedType type);
	/// @brief 经验表包含的植物的数量
	const int PLANT_LEVEL_EXP_LENGTH = 80;
	/// @brief 经验表，升级所需经验,index为当前等级,最大为4
	const std::array<std::array<int, MyPlant::MAX_LEVEL>, PLANT_LEVEL_EXP_LENGTH> PLANT_LEVEL_EXP = { {
		{  500,  1500,  2500,  4000,   7500 },  // 豌豆
		{    0,     0,     0,     0,      0 },  // 麦
		{    0,     0,     0,     0,      0 },  // 樱桃炸弹
		{  200,   600,  1000,  1600,   3000 },  // 坚果
		{  300,   900,  1500,  4500,   7500 },  // 土豆雷
		{ 3000,  9000, 15000, 24000,  45000 },  // 雪豌
		{ 1500,  4500,  7500, 12000,  22500 },  // 大嘴
		{ 2500,  7500, 15000, 25000,  40000 },  // 双发
		{ 2500,  7500, 15000, 30000,  45000 },  // 小喷
		{  750,  2250,  7500, 50000,  75000 },  // 阳光
		{ 5000, 15000, 25000, 40000,  75000 },  // 大喷
		{    0,     0,     0,     0,      0 },  // 墓碑
		{ 1500,  4500,  7500, 12000,  22500 },  // 魅惑
		{ 3000,  9000, 18000, 30000,  48000 },  // 胆小
		{ 1000,  3000,  5000,  8000,  15000 },  // 寒冰菇
		{    0,     0,     0,     0,      0 },  // 毁灭
		{ 3000,  9000, 15000, 24000,  45000 },  // 蒂安希晶钻菇
		{ 3000,  9000, 15000, 24000,  45000 },  // 窝瓜
		{ 5000, 10000, 15000, 20000,  33333 },  // 三线
		{ 1000,  3000,  5000,  8000,  15000 },  // 水草
		{    0,     0,     0,     0,      0 },  // 辣椒
		{    0,     0,     0,     0,      0 },  // 地刺
		{ 2000,  6000, 10000, 16000,  30000 },  // 火炬
		{  200,   600,  1000,  1600,   3000 },  // 高坚果
		{ 3000,  9000, 15000, 24000,  45000 },  // 海蘑菇
		{  750,  2250,  3750,  6000,  11250 },  // 老灯
		{  200,   600,  1000,  1600,   3000 },  // 仙人
		{  750,  2250,  3750,  6000,  11250 },  // 三叶
		{ 3000,  9000, 15000, 24000,  45000 },  // 裂荚
		{ 2000,  8000, 12000, 20000,  25000 },  // 1437
		{    0,     0,     0,     0,      0 },  // 南瓜
		{  500,  1500,  2500,  4000,   7500 },  // 磁力
		{ 3000,  9000, 15000, 24000,  45000 },  // 卷心菜
		{    0,     0,     0,     0,      0 },  // 盆
		{ 3000,  9000, 15000, 24000,  45000 },  // 玉米
		{    0,     0,     0,     0,      0 },  // 咖啡豆
		{  200,   600,  1000,  1600,   3000 },  // 蒜
		{  500,  1500,  2500,  4000,   7500 },  // 伞
		{  750,  2250,  3750,  6000,  11250 },  // 大佬花
		{ 6000, 13500, 24000, 36000,  60000 },  // 西瓜
		{ 5000, 10000, 20000, 40000,  80000 },  // 机枪
		{    0,     0,     0,     0,      0 },  // 双子（牢麦幼体）
		{ 5000, 15000, 25000, 40000,  75000 },  // 曾哥
		{  450,  1350,  4500, 13500,  45000 },  // 香蒲
		{ 1800,  5400,  7200, 14400,  30000 },  // 冰瓜
		{  750,  2250,  3750,  6000,  11250 },  // 吸金磁
		{  200,   600,  1000,  1600,   3000 },  // 地刺王
		{ 5000, 10000, 15000, 20000,  30000 },  // 加农炮
		{ 5000, 10000, 15000, 20000,  30000 },  // imitater（模仿者）
		{  200,   600,  1000,  1600,   3000 },  // 爆炸坚果
		{ 5000, 10000, 15000, 20000,  30000 },  // 巨大坚果（目前没有）
		{ 3000,  9000, 15000, 24000,  45000 },  // 飘飘
		{ 3000,  9000, 15000, 24000,  45000 },  // 反双，编号52
		{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},// 53-75，留空
		{  750,  2250,  3750,  6000,  11250 },  // 春分藤，编号76
		{ 3000,  9000, 15000, 24000,  45000 },  // 嘉伟
		{ 3000,  9000, 15000, 24000,  45000 },  // 山竹
		{  500,  1500,  2500,  4000,   7500 },  // 草时逆！
	} };
}