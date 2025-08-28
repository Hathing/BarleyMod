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
		int GetEasterProbabilityPartition()
		{
			return 0;
		}
	};
	class NilPlant : public NoXPPlant
	{
		int GetEasterProbabilityPartition()
		{
			return 0;
		}
	};
}
