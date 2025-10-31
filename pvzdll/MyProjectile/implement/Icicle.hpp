#pragma once
#include "../ProjectileAbility.hpp"
#include "../../pch.h"

namespace ProjectileAbility
{
	class Icicle : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 20;
		}
		void onDamageZombie(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			zombie.AddFrost(5);
		}
		int GetImage(MyProjectile proj)
		{
			return NewImage::ICICLE.GetBaseAddress();
		}
		bool GetRect(MyProjectile proj, PVZ::Rect* rect)
		{
			rect->X = proj.ImageX;
			rect->Y = proj.ImageY;
			rect->Width = 80;
			rect->Height = 20;
			return false;
		}
	};
}
