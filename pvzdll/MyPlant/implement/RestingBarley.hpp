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
		bool TickAbility(MyPlant plant)
		{
			plant.BarleyCounter++;
			if (plant.BarleyCounter == 3010)
			{
				plant.Remove();
				MyPlant creep = Creator::CreatePlant(SeedType::Barley, plant.Row, plant.Column);
				PVZ::CreateParticleSystem(plant.ImageX + 40.0f, plant.ImageY + 40.0f, 0x618A0, EffectType::IMITATER_TRANSFORMING);
			}

			return true;
		}
	};
}
