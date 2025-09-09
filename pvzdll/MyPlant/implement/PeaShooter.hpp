#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class PeaShooter : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 300, 500, 500, 800, 800, 800 };
		inline static const int base_damage[6] = { 200, 200, 300, 300, 400, 400 };
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductInterval = 700;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}

		bool TickAbility(MyPlant plant)
		{
			if (!plant.AttributeCountdown)
			{
				plant.AttributeCountdown = 1;
				MyZombie zombie = plant.FindTargetZombie(0);
				if (zombie.isValid())
				plant.mTargetZombieID = zombie.Id;
			}
			return false;
		}

		int onFindTargetRT(MyPlant plant, MyZombie zombie, int row)
		{
			if (zombie.Blowaway)
				return ThreeState::Disable;
			if (zombie.BodyHealth < 30 && (zombie.Type == ZombieType::Zomboin || zombie.Type == ZombieType::CatapultZombie))
				return ThreeState::Disable;
			if (!zombie.NotDying)
				return ThreeState::Disable;
			switch (zombie.State)
			{
			case ZombieState::NEWSPAPER_DESTORYED:
			case ZombieState::DIGGER_LOST_DIG:
				return ThreeState::Disable;
			}
			return ThreeState::None;
		}

		bool onFire(MyPlant plant, MyZombie target, int weapon_type)
		{
			MyZombie zombie = PVZ::GetByID<MyZombie>((unsigned int)plant.mTargetZombieID);
			if(!zombie.isValid())
				zombie = plant.FindTargetZombie(0);

			if (zombie.isValid())
			{
				PVZ::ApplyPZDamage(plant, zombie, 500, PVZ::DAMAGEF_BYPASSES_SHIELD);
				//创建特效
				auto particle = PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, 0x61A80, EffectType::ZOMBIE_GET_KERNEL_SHOT);
				particle.OverrideImage(PVZ::Image(Memory::ReadMemory<DWORD>(0x6A76A8)));
				PVZ::CreateParticleSystem(zombie.X + 40.0f, zombie.Y + 65.0f, 0x61A80, EffectType::HAMMER_BANG);
				plant.AttributeCountdown = 50;
				plant.mTargetZombieID = 0;
			}
			Creator::CreateLowerSound(LowerSoundType::CherryExplode);
			return false;
		}

		int GetDamageRangeFlags(MyPlant plant, int weapon_type)
		{
			return int(PVZ::DRF_OFF_GROUND | PVZ::DRF_FLYING | PVZ::DRF_GROUND);
		}
	};
}
