#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include "Const.hpp"
#include "MyEvents.hpp"
#include "MyBoard.hpp"
#include "MyPlant/MyPlant.hpp"
#include "MyProjectile/MyProjectile.hpp"
#include "MyZombie/MyZombie.hpp"

void init();
void InitResourceEvents();
void InitBoardEvents();
void InitLawnmowerEvents();
void InitPlantEvents();
void InitProjectileEvents();

#endif //PCH_H
