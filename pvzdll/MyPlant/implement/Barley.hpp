#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class Barley : public BasePlant
	{
		bool IsXPRecipient(MyPlant plant)
		{
			return false;
		}
	};
}
