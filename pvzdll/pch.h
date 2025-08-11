#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include "Const.hpp"
#include "MyEvents.hpp"
#include "MyBoard.hpp"
#include "MyPlant/MyPlant.hpp"
#include "MyProjectile/MyProjectile.hpp"
#include "MyZombie/MyZombie.hpp"

typedef PVZEvent::PlantDamageZombieEvent::PZDamageInfo<MyPlant, MyZombie> PZDamageEvent;

void init();
void InitResourceEvents();
void InitBoardEvents();
void InitLawnmowerEvents();
void InitPlantEvents();
void InitProjectileEvents();
void InitZombieEvents();

#endif //PCH_H
