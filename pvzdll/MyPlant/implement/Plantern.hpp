#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Plantern : public NoEasterSkinPlant
	{
		inline static const int max_health[6] = { 300, 450, 450, 600, 600, 600 };
		inline static const int heal_cooldown[6] = { 6000, 6000, 4800, 4800, 3600, 3600 };
		inline static constexpr int respawn_cooldown = 18000;
		void onCreated(MyPlant plant)
		{
			plant.Hp = max_health[0];
			plant.MaxHp = max_health[0];
			plant.AttributeCountdown = Creator::Rand(heal_cooldown[0]);
			plant.AnotherCounter = 0;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		bool TickAbility(MyPlant plant)
		{
			if (!plant.OnBoard)
				return false;
			if (!plant.AttributeCountdown)
			{
				plant.AttributeCountdown = heal_cooldown[plant.Level];
				auto plants = plant.GetBoard().GetAllPlants<MyPlant>();
				for (auto myplant : plants)
					if (myplant.Row == plant.Row)
						myplant.Heal(100);
			}
			if (plant.AnotherCounter > 0)
				plant.AnotherCounter -= 1;
			return false;
		}
		bool onAnimate(MyPlant plant)
		{
			auto model = plant.GetAnimationPart1();
			if (model.isValid())
			{
				int partition = plant.Hp * 6 / plant.MaxHp;

				model.AssignRenderGroupToPrefix(partition < 1 ? -1 : 0, "Plantern_leaf3");
				model.AssignRenderGroupToPrefix(partition < 2 ? -1 : 0, "Plantern_leaf1");
				model.AssignRenderGroupToPrefix(partition < 3 ? -1 : 0, "Plantern_leaf4");
				model.AssignRenderGroupToPrefix(partition < 4 ? -1 : 0, "Plantern_leaf2");
				model.AssignRenderGroupToPrefix(partition < 5 ? -1 : 0, "Plantern_leaf5");
			}
			return false;
		}
		bool onPlantDying(MyPlant caster, MyPlant dying_plant, PlantDyingType dying_reason)
		{
			if (caster.Id == dying_plant.Id)
				return true;

			if (caster.Level >= MyPlant::MAX_LEVEL && dying_plant.IsPrime())
			{
				caster.Hp -= 4;
				caster.Light(50);
				dying_plant.Hp = 0;
				dying_plant.Light(50);
				return false;
			}

			return true;
		}
		void onPlantDeath(MyPlant caster, MyPlant dying_plant, PlantDyingType dying_reason)
		{
			if (caster.Id == dying_plant.Id)
				return;

			if (!caster.AnotherCounter)
			{
				caster.Hp = 0;
				caster.AnotherCounter = respawn_cooldown;

				Creator::CreatePlant(dying_plant.Type, dying_plant.Row, dying_plant.Column);
			}
		}
	};
}
