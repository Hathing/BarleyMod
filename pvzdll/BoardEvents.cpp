#include "pch.h"
#include "../pvzclass/Events/BoardInitAfterEvent.hpp"
#include "../pvzclass/Events/UpdateGameObjectsEvent.h"

void onBoardInit(MyBoard board)
{
	board.PoisonCounter = 1;
}

void onBoardUpdateGameObject(MyBoard board)
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

void InitBoardEvents()
{
	PVZEvent::BoardInitAfterEvent((int)onBoardInit);
	UpdateGameObjectsEvent((int)onBoardUpdateGameObject);
}