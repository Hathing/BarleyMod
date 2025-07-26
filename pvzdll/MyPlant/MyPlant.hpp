#pragma once
#include "../framework.h"

class MyPlant : public PVZ::Plant
{
public:
	MyPlant(int idoraddress) : PVZ::Plant(idoraddress) {};
	MyPlant(const PVZ::Plant& plant) : PVZ::Plant(plant.GetBaseAddress()) {};

	void Heal(int val);

	static const int MAX_LEVEL = 5;

	MyPlant GetByID(int id);
};