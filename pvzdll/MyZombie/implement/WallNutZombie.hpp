#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class WallNutZombie : public BaseZombie
	{
	public:
		void onCreated(MyZombie zombie)
		{
			auto model = zombie.GetSpecialHeadAnimation();
			model.AssignRenderGroupToPrefix(-1, "awake");
			model.AssignRenderGroupToPrefix(-1, "easter");
		}
		bool OverrideHelmDamageTexture(MyZombie zombie, PVZ::Animation reanim, int partition)
		{
			if (zombie.VariantType == ZombieVariantType::GarlicHead)
			{
				auto head = zombie.GetSpecialHeadAnimation();
				switch (partition)
				{
				case 1:
					head.SetImageOverride("anim_face", *((PVZ::Image*)0x6A74B8));
					return false;
				case 2:
					head.SetImageOverride("anim_face", *((PVZ::Image*)0x6A73A8));
					return false;
				default:
					return false;
				}
				return false;
			}
			return true;
		}
	};
}