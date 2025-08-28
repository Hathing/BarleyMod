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
	class NoEasterSkinPlant : public BasePlant
	{
		virtual int GetEasterProbabilityPartition()
		{
			return 0;
		}
	};
}
