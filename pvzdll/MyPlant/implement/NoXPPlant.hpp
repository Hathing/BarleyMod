#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class NoXPPlant : public BasePlant
	{
		bool IsXPRecipient(MyPlant plant)
		{
			return false;
		}
	};
}
