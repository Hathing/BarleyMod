#pragma once
#include "MyPlant.hpp"
#include "../../MyZombie/MyZombie.hpp"

namespace PlantAbility
{
	class BasePlant
	{
	public:
		virtual bool TickAbility(MyPlant plant)
		{
			return true;
		}
	};
	typedef BasePlant* PlantPTR;
	PlantPTR GetPrototype(SeedType::SeedType type);
}
