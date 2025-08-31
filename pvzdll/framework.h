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

/// @brief 各种子弹的特殊类型
typedef unsigned char ProjSpecialType;
constexpr ProjSpecialType PST_NONE = 0;
/// @brief 橙色炎弹/过火变橙炎弹的豌豆
constexpr ProjSpecialType PST_ORANGE_FIREBALL = 1;
/// @brief 红色炎弹/过火变红炎弹的豌豆
constexpr ProjSpecialType PST_RED_FIREBALL = 2;
/// @brief 蓝色炎弹/过火变蓝炎弹的豌豆
constexpr ProjSpecialType PST_BLUE_FIREBALL = 3;
/// @brief 紫色炎弹/过火变紫炎弹的豌豆
constexpr ProjSpecialType PST_PURPLE_FIREBALL = 4;
/// @brief 裂荚豌豆的爆裂子弹
constexpr ProjSpecialType PST_CRACK_PEA = 5;
/// @brief 冰瓜的大炮
constexpr ProjSpecialType PST_CANNON_MELON = 1;


constexpr int WAVE_ELITE1 = 5001;