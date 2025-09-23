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
	/// @brief 子弹的动画附件的ID
	INT_PROPERTY(AttachmentID, __get_AttachmentID, __set_AttachmentID, 0x7C);
	/// @brief 创建该子弹的植物的 ID
	INT_PROPERTY(ParentID, __get_PaID, __set_PaID, 0x80);
	/// @brief 杨桃创建该子弹时子弹所在的行
	T_PROPERTY(byte,OriginalRow, __get_OrR, __set_OrR, 0x84);
	/// @brief 子弹的特殊标记
	T_PROPERTY(ProjSpecialFlags, SpecialFlags, __get_SpecialFlags, __set_SpecialFlags, 0x84);
	/// @brief 三线创建子弹时子弹的初始Y坐标，使用了子弹的Z加速度，因此注意其他运动方式对该参数的影响
	T_PROPERTY(float, OriginalY, __get_OriginalY, __set_OriginalY, 0x48);
	/// @brief 抛射子弹的弹跳计数器
	T_PROPERTY(byte, BounceCount, __get_BounceCount, __set_BounceCount, 0x85);
	/// @brief 子弹的特殊层数，如西瓜大小
	T_PROPERTY(byte, SpecialStack, __get_SpecialStack, __set_SpecialStack, 0x86);
	/// @brief 子弹的特殊类型，如火球
	T_PROPERTY(byte, SpecialType, __get_SpecialType, __set_SpecialType, 0x87);
	/// @brief 穿透子弹的“幽灵子弹”的地址（非ID）
	INT_PROPERTY(GhostAddr, __get_GhostID, __set_GhostID, 0x88);
	/// @brief 根据子弹坐标调整子弹行数，目前只有三线子弹在用，其余子弹要用的话请注意
	void AdjustRow();
	/// @brief 生成一个不会实际更新的“幽灵子弹”，用于记录穿透的僵尸
	/// @attention 并不能阻止其他函数访问这个子弹，尤其是遍历！
	/// @param piercingnum 能够穿透的目标的最大数量
	void MakePiercing(int piercingnum);
};