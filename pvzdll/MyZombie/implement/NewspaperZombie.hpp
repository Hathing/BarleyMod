#pragma once
#include "../ZombieAbility.hpp"

namespace ZombieAbility
{
	class NewspaperZombie : public BaseZombie
	{
		void onCreated(MyZombie zombie)
		{
			zombie.BodyHealth = 270;
			zombie.BodyMaxHealth = 270;
			zombie.ShieldHealth = 150;
			zombie.ShieldMaxHealth = 150;

			if (zombie.FromWave >= WAVE_ELITE_MASK)
			{
				zombie.BodyHealth = 360;
				zombie.BodyMaxHealth = 360;
			}
		}
		AnimationType::AnimationType GetReanimType(MyZombie zombie, AnimationType::AnimationType type)
		{
			return zombie.FromWave == WAVE_ELITE1 ? (AnimationType::AnimationType)0x0A2 : type;
		}
		void OverwritePZDamage(PZDamageEvent* info)
		{
			int flag = info->flags;
			if (flag & PVZ::DAMAGEF_BYPASSES_SHIELD)
				flag -= PVZ::DAMAGEF_BYPASSES_SHIELD;
			if (flag & PVZ::DAMAGEF_HITS_SHIELD_AND_BODY)
				flag -= PVZ::DAMAGEF_HITS_SHIELD_AND_BODY;

			info->flags = (PVZ::DamageFlags)flag;

			return;
		}
	};
}
