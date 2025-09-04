#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SpringEquinoxVine : public BasePlant
	{
		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductCountdown = 0;
			plant.ShootOrProductInterval = 0;
		}
		int GetEasterProbabilityPartition()
		{
			return 1;
		}
		void onDie(MyPlant plant)
		{
			auto griditem = PVZ::GetByID<PVZ::Griditem>(plant.RelatedGriditemID1);
			if (griditem.isValid())
				griditem.Remove();

			griditem = PVZ::GetByID<PVZ::Griditem>(plant.RelatedGriditemID2);
			if (griditem.isValid())
				griditem.Remove();
		}
	};
}
