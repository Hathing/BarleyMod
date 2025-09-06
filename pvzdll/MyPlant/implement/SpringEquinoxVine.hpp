#pragma once
#include "../PlantAbility.hpp"

namespace PlantAbility
{
	class SpringEquinoxVine : public BasePlant
	{
		inline static const int max_health[6] = { 300, 300, 600, 600, 1000, 1000 };

		/// @brief 清理植物的特殊地砖
		/// @param plant 植物
		static void ClearGrids(MyPlant plant)
		{
			auto griditem = PVZ::GetByID<PVZ::Griditem>(plant.RelatedGriditemID1);
			if (griditem.isValid())
				griditem.Remove();

			griditem = PVZ::GetByID<PVZ::Griditem>(plant.RelatedGriditemID2);
			if (griditem.isValid())
				griditem.Remove();
		}

		void onCreated(MyPlant plant)
		{
			plant.ShootOrProductCountdown = 0;
			plant.ShootOrProductInterval = 0;
			plant.SetMaxHealth(max_health[0]);

			if (plant.OnBoard)
			{
				MyBoard board = plant.GetBoard();
				plant.RelatedGriditemID1 = board.AddGriditem(GriditemType::SpringEquinoxFireGrid, plant.Row, plant.Column).Id;
				plant.RelatedGriditemID2 = board.AddGriditem(GriditemType::SpringEquinoxIceGrid, plant.Row, plant.Column).Id;
			}
		}
		void onUpgrade(MyPlant plant)
		{
			plant.SetMaxHealth(max_health[plant.Level]);
			if (plant.Level == MyPlant::MAX_LEVEL)
				ClearGrids(plant);
		}
		int GetEasterProbabilityPartition()
		{
			return 1;
		}
		void onDie(MyPlant plant)
		{
			ClearGrids(plant);
		}
	};
}
