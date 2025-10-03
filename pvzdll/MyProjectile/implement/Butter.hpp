#pragma once
#include "../ProjectileAbility.hpp"

namespace ProjectileAbility
{
	class Butter : public BaseProjectile
	{
	public:
		float GetImageSize(MyProjectile proj, float original_val)
		{
			if (proj.SpecialFlags == 1)
				original_val = 0.72f;
			if (proj.SpecialStack == 1)
				original_val *= 2;
			return original_val;
		}
		void onImpact(MyProjectile proj, MyZombie zombie)
		{
			if (proj.SpecialStack == 1)
			{
				auto zombies = proj.GetBoard().GetAllZombies<MyZombie>();
				for (auto& myzombie : zombies)
					if (myzombie.Row == proj.Row)
						if (myzombie.X >= proj.X - 20 && myzombie.X <= proj.X + 60)
							myzombie.Butter(400);
			}
		}
	};
}
