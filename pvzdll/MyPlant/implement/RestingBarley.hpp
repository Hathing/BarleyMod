#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class RestingBarley : public BasePlant
	{
		bool IsXPRecipient(MyPlant plant)
		{
			return false;
		}
	};
}
