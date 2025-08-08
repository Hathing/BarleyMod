#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class GloomShroom : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.Hp = 800;
			plant.MaxHp = 800;
		}
	};
}
