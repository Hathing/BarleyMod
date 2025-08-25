#include "pch.h"

void InitPlantExistCount(MyBoard& board)
{
	std::fill(MyBoard::GoldMagnetFactors.begin(), MyBoard::GoldMagnetFactors.end(), 1.0f);
	std::fill(MyBoard::IceShroomCounts.begin(), MyBoard::IceShroomCounts.end(), false);
}

void onBoardInit(MyBoard board)
{
	board.PoisonCounter = 1;
	InitPlantExistCount(board);
}

inline void UpdatePoisonApply(MyBoard& board)
{
	board.PoisonCounter++;
	if (board.PoisonCounter == 10)
	{
		board.PoisonCounter = 0;

		auto zombies = board.GetAllZombies<MyZombie>();
		for (auto& zombie : zombies)
		{
			if (!zombie.PoisonStack)
				continue;
			if (zombie.Type == ZombieType::DrZomboss)
				continue;
			if (zombie.Type == ZombieType::NewspaperZombie && zombie.EliteType)
				continue;
			zombie.Hit(zombie.PoisonStack, PVZ::DAMAGEF_NOFLASH);
		}
	}
}

void UpdatePlantExistCount(MyBoard& board)
{
	InitPlantExistCount(board);
	auto plants = board.GetAllPlants<MyPlant>();
	for (auto& plant : plants)
	{
		const int row = plant.Row, col = plant.Column;
		const auto type = plant.Type;
		if (type == SeedType::GoldMagnet) MyBoard::GoldMagnetFactors[row] *= 1.5f;
		if (type == SeedType::Iceshroom) MyBoard::IceShroomCounts[row] = true;
	}
}

void onBoardUpdateGameObject(MyBoard board)
{
	UpdatePoisonApply(board);
	UpdatePlantExistCount(board);
}

void InitBoardEvents()
{
	PVZEvent::BoardInitAfterEvent((int)onBoardInit);
	UpdateGameObjectsEvent((int)onBoardUpdateGameObject);
}