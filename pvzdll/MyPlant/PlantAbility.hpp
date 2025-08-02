#pragma once
#include "MyPlant.hpp"
#include "../MyZombie/MyZombie.hpp"

namespace PlantAbility
{
	class BasePlant
	{
	public:
		virtual bool TickAbility(MyPlant plant)
		{
			return true;
		}
		virtual bool OverwritePlantAttackRect(MyPlant plant, bool secondary, PVZ::Rect* rect)
		{
			return false;
		}
	};
	typedef BasePlant* PlantPTR;
	PlantPTR GetPrototype(SeedType::SeedType type);
}
