#pragma once
#include "framework.h"

constexpr int MAX_BOARD_ROWS = 6;

class MyBoard : public PVZ::Board
{
public:
	MyBoard(int address) : PVZ::Board(address) {};
	MyBoard(const PVZ::Board& board) : PVZ::Board(board.GetBaseAddress()) {};

	/// @brief 每行吸金磁的倍数
	inline static std::array<float, MAX_BOARD_ROWS> GoldMagnetFactors{};
	/// @brief 每行寒冰菇是否存在
	inline static std::array<bool, MAX_BOARD_ROWS> IceShroomCounts{};
	/// @brief 每行三叶草加速点数
	inline static std::array<int, MAX_BOARD_ROWS> BloverAccelerateCounts{};
	/// @brief 对局计时
	INT_PROPERTY(MatchTimer,__get_MatchTimer,__set_MatchTimer,0x5800);
	/// @brief 对局是否正在进行
	T_PROPERTY(bool,MatchRunning, __get_MatchRunning, __set_MatchRunning, 0x5804);
	/// @brief 周期为10cs的计时器
	T_PROPERTY(byte, Timer10cs, __get_Timer10cs, __set_Timer10cs, 0x5805);
	/// @brief 周期为1s的计时器
	T_PROPERTY(byte, Timer1s, __get_Timer1s, __set_Timer1s, 0x5806);
	/// @brief 植物可以更新技能
	T_PROPERTY(bool, EnablePlantAbility, __get_EnablePlantAbility, __set_EnablePlantAbility, 0x5807);
	/// @brief 僵尸毒结算倒计时
	T_PROPERTY(byte, PoisonCounter, __get_PoC, __set_PoC, 0x165);
	/// @brief 僵尸颜色显示切换计时
	T_PROPERTY(byte, ZombieColorShiftCounter, __get_ZombieColorShiftCounter, __set_ZombieColorShiftCounter, 0x166);

	/// @brief 在指定行右侧生成一只指定类型的僵尸
	/// @param type 僵尸类型
	/// @param row 行
	/// @param from_wave 僵尸所在的波数
	/// @return 生成的僵尸
	PVZ::Zombie AddZombieInRow(ZombieType::ZombieType type, int row, int from_wave);

	PVZ::Griditem AddGriditem(GriditemType::GriditemType type, int row, int column);

	int PixelToGridX(int x, int y);
	int PixelToGridXKeepOnBoard(int x, int y);
	int PixelToGridY(int x, int y);
	int PixelToGridYKeepOnBoard(int x, int y);

	void GetPlantsOnLawn(int x, int y, PlantOnLawn& plants);
};