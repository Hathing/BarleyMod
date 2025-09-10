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

void onLawnmowerMowZombie(PVZ::LawnMower mower, MyZombie zombie)
{
	zombie.LastDamageSourceID = 0;
}

void InitLawnmowerEvents()
{
	LawnmowerUpdateEvent((int)onLanwmowerUpdate);
	PVZEvent::LawnmowerMowZombieEvent((int)onLawnmowerMowZombie);
}