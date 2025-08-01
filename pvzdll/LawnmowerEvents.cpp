#include "pch.h"

bool onLanwmowerUpdate(PVZ::LawnMower mower)
{
	if (mower.State == LawnmoverState::Triggered)
	{
		auto plants = PVZ::GetBoard().GetAllPlants<MyPlant>();
		int r = mower.X + 20, l = mower.X - 80;
		for (auto& plant : plants)
			if (plant.Row == mower.Row && plant.ImageX >= l && plant.ImageX <= r)
				plant.Smash();
	}
	return true;
}

void InitLawnmowerEvents()
{
	LawnmowerUpdateEvent((int)onLanwmowerUpdate);
}