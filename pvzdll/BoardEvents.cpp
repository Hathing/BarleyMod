#include "pch.h"
#include "../pvzclass/Events/BoardInitAfterEvent.hpp"
#include "../pvzclass/Events/UpdateGameObjectsEvent.h"

void onBoardInit(MyBoard board)
{
	board.PoisonCounter = 1;
}

void InitBoardEvents()
{
	PVZEvent::BoardInitAfterEvent((int)onBoardInit);
}