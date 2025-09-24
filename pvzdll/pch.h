#ifndef PCH_H
#define PCH_H

#include "framework.h"
#include "Const.hpp"
#include "MyEvents.hpp"
#include "MyClasses.hpp"

void init();
void InitResourceEvents();
void InitBoardEvents();
void InitLawnmowerEvents();
void InitPlantEvents();
void InitProjectileEvents();
void InitZombieEvents();
void InitCoinEvents();
void InitRandomZombieEvents();

#endif //PCH_H
