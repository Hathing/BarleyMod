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
	/// @brief 僵尸毒结算倒计时
	T_PROPERTY(byte, PoisonCounter, __get_PoC, __set_PoC, 0x165);

	/// @brief 对指定行的所有僵尸造成灰烬伤害
	/// @param row 行
	void BurnRow(int row);
};