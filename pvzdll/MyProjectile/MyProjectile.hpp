#pragma once
#include "../framework.h"

class MyProjectile : public PVZ::Projectile
{
public:
	MyProjectile(int idoraddress) : PVZ::Projectile(idoraddress) {};
	MyProjectile(const PVZ::Projectile& proj) : PVZ::Projectile(proj.GetBaseAddress()) {};

	/// @brief 穿透子弹使用的“幽灵子弹”的标记
	T_PROPERTY(bool, IsGhost, __get_IsGhost, __set_IsGhost, 0x19);
	/// @brief 子弹生成时，来源植物的等级
	T_PROPERTY(byte, SourceLevel, __get_SoL, __set_SoL, 0x51);
	/// @brief 子弹生成时，来源植物的类型
	T_PROPERTY(byte, SourceType, __get_SoT, __set_SoT, 0x52);
	/// @brief 一个自减倒计时，暂时不清楚会不会影响原版子弹的更新
	INT_PROPERTY(DecrementTime, __get_DecrementTime, __set_DecrementTime, 0x64);
	/// @brief 小喷菇生成子弹时孢子的倍率，现已弃用，仅供阅读CT参考！
	/// @deprecated
	INT_PROPERTY(Unknown2, __get_Un2, __set_Un2, 0x78);
	/// @brief 小喷菇五阶技能属性，子弹的额外基础倍率而非随机倍率
	T_PROPERTY(float, PuffShroomBaseMultiplier, __get_PuffShroomBaseMultiplier, __set_PuffShroomBaseMultiplier, 0x78);
	/// @brief 豌豆和火球最后一次过火的列号
	INT_PROPERTY(LastOnFireColumn, __get_LastOnFireColumn, __set_LastOnFireColumn, 0x78);
	/// @brief 子弹的动画附件的ID
	INT_PROPERTY(AttachmentID, __get_AttachmentID, __set_AttachmentID, 0x7C);
	/// @brief 创建该子弹的植物的 ID
	INT_PROPERTY(ParentID, __get_PaID, __set_PaID, 0x80);
	/// @brief 杨桃创建该子弹时子弹所在的行
	T_PROPERTY(byte,OriginalRow, __get_OrR, __set_OrR, 0x84);
	/// @brief 子弹的特殊标记
	T_PROPERTY(ProjSpecialFlags, SpecialFlags, __get_SpecialFlags, __set_SpecialFlags, 0x84);
	INT_PROPERTY(Unknown, __get_Un, __set_Un, 0x8C);
	/// @brief 三线创建子弹时子弹的初始Y坐标，使用了子弹的Z加速度，因此注意其他运动方式对该参数的影响
	T_PROPERTY(float, OriginalY, __get_OriginalY, __set_OriginalY, 0x48);
	/// @brief 抛射子弹的弹跳计数器
	T_PROPERTY(byte, BounceCount, __get_BounceCount, __set_BounceCount, 0x85);
	/// @brief 子弹的特殊层数，如西瓜大小，小喷菇随机倍率
	T_PROPERTY(byte, SpecialStack, __get_SpecialStack, __set_SpecialStack, 0x86);
	/// @brief 子弹的特殊类型，如火球
	T_PROPERTY(byte, SpecialType, __get_SpecialType, __set_SpecialType, 0x87);
	/// @brief 穿透子弹的“幽灵子弹”的ID
	INT_PROPERTY(GhostID, __get_GhostID, __set_GhostID, 0x88);

	/// @brief “幽灵子弹”最大可穿透数量
	INT_PROPERTY(PiercingMaxCount, __get_PiercingMaxCount, __set_PiercingMaxCount, 0x68);
	/// @brief “幽灵子弹”已穿透数量
	INT_PROPERTY(PiercingCount, __get_PiercingCount, __set_PiercingCount, 0x6C);
	/// @brief “幽灵子弹”记录的第一个目标的ID
	INT_PROPERTY(PiercingID1, __get_PiercingID1, __set_PiercingID1, 0x30);
	/// @brief “幽灵子弹”记录的第二个目标的ID
	INT_PROPERTY(PiercingID2, __get_PiercingID2, __set_PiercingID2, 0x34);
	/// @brief “幽灵子弹”记录的第三个目标的ID
	INT_PROPERTY(PiercingID3, __get_PiercingID3, __set_PiercingID3, 0x38);
	/// @brief “幽灵子弹”记录的第四个目标的ID
	INT_PROPERTY(PiercingID4, __get_PiercingID4, __set_PiercingID4, 0x3C);
	/// @brief “幽灵子弹”记录的第五个目标的ID
	INT_PROPERTY(PiercingID5, __get_PiercingID5, __set_PiercingID5, 0x40);
	/// @brief “幽灵子弹”记录的第六个目标的ID
	INT_PROPERTY(PiercingID6, __get_PiercingID6, __set_PiercingID6, 0x44);
	/// @brief “幽灵子弹”记录的第七个目标的ID
	INT_PROPERTY(PiercingID7, __get_PiercingID7, __set_PiercingID7, 0x48);
	/// @brief “幽灵子弹”记录的第八个目标的ID
	INT_PROPERTY(PiercingID8, __get_PiercingID8, __set_PiercingID8, 0x4C);
	/// @brief “幽灵子弹”记录的目标，index最大为8
	INT_ARRAY_PROPERTY(GetPiercingID, SetPiercingID, 0x30);

	/// @brief 调用原函数获取伤害flag
	PVZ::DamageFlags GetDamageFlags(int zombie_addr);
	/// @brief 根据子弹坐标调整子弹行数，目前只有三线子弹在用，其余子弹要用的话请注意
	void AdjustRow();
	/// @brief 生成一个不会实际更新的“幽灵子弹”，用于记录穿透的僵尸
	/// @attention 并不能阻止其他函数访问这个子弹，尤其是遍历！
	/// @param piercingnum 能够穿透的目标的最大数量，目前最大允许8个目标
	/// @param xspeed 子弹的X速度
	/// @param yspeed 子弹的Y速度
	void MakePiercing(int piercingnum, float xspeed, float yspeed);
	/// @brief 从其他子弹上面继承部分初始化属性
	/// @param proj 其他子弹
	void DeriveProperty(MyProjectile proj);
};