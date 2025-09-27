#pragma once

#include <windows.h>
#include "../pvzclass/pvzclass.h"
#include "../pvzclass/Events/Events.h"

extern bool CLOWN_ZOMBIE_POP_FLAG;

typedef bool mybool;

struct PlantOnLawn
{
	PVZ::Plant BottomPlant;
	PVZ::Plant PumpkinPlant;
	PVZ::Plant FlyingPlant;
	PVZ::Plant MainPlant;
	PlantOnLawn(): BottomPlant(INVALID_BASEADDRESS), PumpkinPlant(INVALID_BASEADDRESS), FlyingPlant(INVALID_BASEADDRESS), MainPlant(INVALID_BASEADDRESS){}
};

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
/// @brief 裂荚豌豆的散开的小子弹
constexpr ProjSpecialType PST_SCATTER_PEA = 6;
/// @brief 冰瓜的小散弹
constexpr ProjSpecialType PST_SCATTER_WINTERMELON = 1;
/// @brief 冰瓜的大炮
constexpr ProjSpecialType PST_CANNON_WINTERMELON = 2;

constexpr int WAVE_ELITE_MASK = 5000;
constexpr int WAVE_ELITE1 = WAVE_ELITE_MASK + 1;

enum PlantDyingType
{
	DYING_NONE = 0,
	DYING_CLOWN_EXPLODED,
	DYING_SPIKEROCK_SMASHED,
	DYING_SPIKEWEED_ATTACK_CAR,
	DYING_SQUASH_SQUISHING,
	DYING_DISAPPEARING,//包括被碾压后消失、大部分一次性植物在+4C=0时触发
	DYING_HP_BELOW_ZERO,
	DYING_JALAPENO_EXPLODING,
	DYING_ICESHROOM_EXPLODING,
	DYING_CHERRYBOOM_EXPLODING,
	DYING_JALAPENOHEAD_EXPLODED,
	DYING_EATEN,
	DYING_HYPNOSHROOM_EATEN,
	DYING_BUNGEE_STOLEN
};

/// @brief 非精英僵尸的变种类型
namespace ZombieVariantType
{
	typedef unsigned char ZombieVariantType;

	constexpr ZombieVariantType None = 0;

	constexpr ZombieVariantType SnowPeaHead = 1;

}