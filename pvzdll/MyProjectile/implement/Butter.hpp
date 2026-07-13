#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Butter : public BaseProjectile
	{
	public:
		int OverrideDamage(MyProjectile proj, MyZombie zombie, PVZEvent::ProjDmgType damage_type, int subtarget_count, int ori_dmg)
		{
			return 40;
		}
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SpecialFlags == 1)
				original_val = 0.72f;
			if (proj.SpecialStack == 1)
				original_val *= 2;
			return original_val;
		}
		bool onImpact(MyProjectile proj, MyZombie zombie)
		{
			//给玉米增加层数
			auto owner = PVZ::GetByID<MyPlant>(proj.ParentID);
			if (owner.isValid())
			{
				if (owner.Type == SeedType::Kernelpult)
				{
					if(zombie.isValid())
						owner.ButterHitCount++;
				}
			}

			//大黄油特性
			if (proj.SpecialStack == 1)
			{
				auto zombies = proj.GetBoard().GetAllZombies<MyZombie>();
				for (auto& myzombie : zombies)
					if (myzombie.Row == proj.Row)
						if (myzombie.X >= proj.X - 20 && myzombie.X <= proj.X + 60)
							myzombie.Butter(400);
			}
			return true;
		}
	};
}
