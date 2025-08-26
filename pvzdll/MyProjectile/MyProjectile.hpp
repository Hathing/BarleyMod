#pragma once
#include "../framework.h"

class MyProjectile : public PVZ::Projectile
{
public:
	MyProjectile(int idoraddress) : PVZ::Projectile(idoraddress) {};
	MyProjectile(const PVZ::Zombie& zombie) : PVZ::Projectile(zombie.GetBaseAddress()) {};

	/// @brief 一个自减倒计时，暂时不清楚会不会影响原版子弹的更新
	INT_PROPERTY(DecrementTime, __get_DecrementTime, __set_DecrementTime, 0x64);
	/// @brief 创建该子弹的植物的 ID
	INT_PROPERTY(ParentID, __get_PaID, __set_PaID, 0x80);
	/// @brief 创建该子弹时子弹所在的行
	T_PROPERTY(byte,OriginalRow, __get_OrR, __set_OrR, 0x84);
	/// @brief 抛射子弹的弹跳计数器
	T_PROPERTY(byte, BounceCount, __get_BounceCount, __set_BounceCount, 0x85);
};