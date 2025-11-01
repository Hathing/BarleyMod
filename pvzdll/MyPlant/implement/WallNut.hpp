#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class WallNut : public BasePlant
	{
		inline static const int max_health[6] = { 2000, 3500, 4500, 6000, 8000, 8000 };
		inline static constexpr int shield_cooldown = 18000;

		/// @brief 为植物所在行的所有未被碾压的主植物施加护盾
		/// @param plant 触发该效果的植物
		static void ApplyDamageAbsorption(MyPlant plant)
		{
			PVZ::CreateParticleSystem(plant.ImageX + 40.0, plant.ImageY + 40.0, plant.Layer + 1,
				EffectType::COB_CANNON_EXPLODED2).OverrideImage(*((PVZ::Image*)0x6FF118));
			auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
			for (auto& myplant : plants)
				if (myplant.Row == plant.Row && myplant.IsPrime() && !myplant.Squash)
					myplant.DamageAbsorption = 200;
		}

		void onCreated(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[0]);
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
			if (plant.Level == MyPlant::MAX_LEVEL)
			{
				ApplyDamageAbsorption(plant);
				plant.AttributeCountdown = shield_cooldown;
			}
		}
		void SelfHeal(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				plant.Heal(plant.Hp * 3 < plant.MaxHp ? 300 : 150);
			}
			else
			{
				BasePlant::SelfHeal(plant);
			}
		}
		bool TickAbility(MyPlant plant)
		{
			if (plant.AttributeCountdown == 0)
			{
				ApplyDamageAbsorption(plant);
				plant.AttributeCountdown = shield_cooldown;
			}
			return false;
		}
		bool onSquishedByZombie(MyPlant plant, MyZombie zombie)
		{
			return PVZ::ApplyZPDamage(zombie, plant, 2000);
		}
		bool onAnimate(MyPlant plant)
		{
			if (plant.Level >= MyPlant::MAX_LEVEL)
			{
				auto model = plant.GetAnimationPart1();
				if (model.isValid())
				{
					if (plant.Hp * 3 < plant.MaxHp)
						model.SetImageOverride("awaken_kuijia", *((PVZ::Image*)0x6FF048));
					else if (plant.Hp * 3 <= plant.MaxHp * 2)
						model.SetImageOverride("awaken_kuijia", *((PVZ::Image*)0x6FF044));
					else
						model.SetImageOverride("awaken_kuijia", PVZ::Image(0));
					return false;
				}
			}
			return true;
		}
	};
}
