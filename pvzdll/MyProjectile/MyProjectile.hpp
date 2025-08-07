#pragma once
#include "../framework.h"

class MyProjectile : public PVZ::Projectile
{
public:
	MyProjectile(int idoraddress) : PVZ::Projectile(idoraddress) {};
	MyProjectile(const PVZ::Zombie& zombie) : PVZ::Projectile(zombie.GetBaseAddress()) {};

	/// @brief 创建该子弹的植物的 ID
	INT_PROPERTY(ParentID, __get_PaID, __set_PaID, 0x80);
};