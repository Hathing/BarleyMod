#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class TallNut : public BasePlant
	{
		inline static const int max_health[6] = { 1000, 1500, 2000, 3000, 4000, 4000 };
		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
			plant.AttributeCountdown = 50;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		void SelfHeal(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.Heal(50);
			}
			else
			{
				BasePlant::SelfHeal(plant);
			}
		}
		bool TickAbility(MyPlant plant)
		{
			if (!plant.AttributeCountdown)
			{
				plant.AttributeCountdown = 50;
				auto zombies = plant.GetBoard().GetAllZombies<MyZombie>();
				MyZombie target = INVALID_BASEADDRESS;
				for (auto _zombie : zombies)
				{
					if (_zombie.Row == plant.Row && !_zombie.Hypnotized && !_zombie.Blowaway && _zombie.NotDying
						&& _zombie.Type != ZombieType::Zomboin && _zombie.Type != ZombieType::DiggerZombie
						&& _zombie.Type != ZombieType::PogoZombie && _zombie.Type != ZombieType::BungeeZombie
						&& _zombie.Type != ZombieType::CatapultZombie && _zombie.Type != ZombieType::DancingZombie
						&& _zombie.State != ZombieState::POLE_VALUTING_JUMPPING && _zombie.State != ZombieState::BALLOON_FLYING
						&& _zombie.State != ZombieState::IMP_FLYING)
					{
						if (_zombie.Id == plant.mTargetZombieID)
							return false;
						if (_zombie.Taunted)
							continue;

						if (!target.isValid() || target.ImageX > _zombie.ImageX)
							target = _zombie;
					}
				}
				if (target.isValid())
				{
					plant.mTargetZombieID = target.Id;
					target.Taunted = true;
					if (target.ImageX < plant.ImageX)
						target.IsWalkingBackwards = 1;
				}
			}
			return false;
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				int partition = plant.Hp * 3 / plant.MaxHp;
				auto model = plant.GetAnimationPart1();
				if (model.isValid())
				{
					switch (partition)
					{
					case 0:
						model.SetImageOverride("awaken_2", *((PVZ::Image*)0x6FF0B8));
						break;
					case 1:
						model.SetImageOverride("awaken_2", *((PVZ::Image*)0x6FF0B8));
						break;
					case 2:
					default:
						model.SetImageOverride("awaken_2", PVZ::Image(0));
					}
					return false;
				}
			}
			return true;
		}
	};
}
