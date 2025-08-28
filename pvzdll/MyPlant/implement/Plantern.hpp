#pragma once
#include "../PlantAbility.hpp"
#include "SpecialPlants.hpp"

namespace PlantAbility
{
	class Plantern : public NoEasterSkinPlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 4000;
			plant.MaxHp = 4000;
			plant.RespawnType = SeedType::None;
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
	};
}
