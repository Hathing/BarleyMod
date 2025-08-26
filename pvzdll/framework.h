#pragma once

#include <windows.h>
#include "../pvzclass/pvzclass.h"
#include "../pvzclass/Events/Events.h"

typedef bool mybool;


/// @brief 各种子弹的特殊标记
typedef unsigned char ProjSpecialFlags;
/// @brief 三线发射子弹的标记，向边路运动
constexpr ProjSpecialFlags PSF_THREEPEATER_SLIDE_OUT = 51;
/// @brief 三线发射子弹的标记，向中路运动
constexpr ProjSpecialFlags PSF_THREEPEATER_SLIDE_IN = 52;