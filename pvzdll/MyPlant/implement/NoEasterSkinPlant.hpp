#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class NoEasterSkinPlant : public BasePlant
	{
		virtual int GetEasterProbabilityPartition()
		{
			return 0;
		}
	};
}
