#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class IceShroom : public BasePlant
	{
		inline static const int max_health[6] = { 600, 850, 850, 1100, 1100, 1700 };
		void onCreated(MyPlant plant)
		{
			plant.Hp = max_health[0];	
			plant.MaxHp = max_health[0];
			plant.SubClass = 2;
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
		}
		bool TickAbility(MyPlant plant)
		{
			return false;
		}
	};
}
