#include "pch.h"

void InitPlantExistCount(MyBoard& board)
{
	std::fill(MyBoard::GoldMagnetFactors.begin(), MyBoard::GoldMagnetFactors.end(), 1.0f);
	std::fill(MyBoard::IceShroomCounts.begin(), MyBoard::IceShroomCounts.end(), false);
}

void onBoardInit(MyBoard board)
{
	board.PoisonCounter = 1;
	InitPlantExistCount(board);
}

inline void UpdatePoisonApply(MyBoard& board)
{
	board.PoisonCounter++;
	if (board.PoisonCounter == 10)
	{
		board.PoisonCounter = 0;

		auto zombies = board.GetAllZombies<MyZombie>();
		for (auto& zombie : zombies)
		{
			if (!zombie.PoisonStack)
				continue;
			if (zombie.Type == ZombieType::DrZomboss)
				continue;
			if (zombie.Type == ZombieType::NewspaperZombie && zombie.EliteType)
				continue;
			zombie.Hit(zombie.PoisonStack, PVZ::DAMAGEF_NOFLASH);
		}
	}
}

void UpdatePlantExistCount(MyBoard& board)
{
	InitPlantExistCount(board);
	auto plants = board.GetAllPlants<MyPlant>();
	for (auto& plant : plants)
	{
		const int row = plant.Row, level = plant.Level;
		const auto type = plant.Type;
		if (type == SeedType::GoldMagnet)
		{
			if (level >= 5)MyBoard::GoldMagnetFactors[row] *= 2.0f;
			else MyBoard::GoldMagnetFactors[row] *= 1.5f;
		}
		if (type == SeedType::Iceshroom) MyBoard::IceShroomCounts[row] = true;
	}
}

void onBoardUpdateGameObject(MyBoard board)
{
	UpdatePoisonApply(board);
	UpdatePlantExistCount(board);
}

inline int GridToPixelX(int row, int col, MyBoard& board)
{
	//不适用于禅境花园
	return (80 * col + 40);
}

inline int GridToPixelY(int row, int col, MyBoard& board)
{
	//不适用于禅境花园、原版废稿高地
	if (board.LevelScene == SceneType::Pool || board.LevelScene == SceneType::Pool)
		return row * 85 + 50;
	else if (board.LevelScene == SceneType::Day || board.LevelScene == SceneType::Night)
		return row * 100 + 50;
	else if (board.LevelScene == SceneType::Roof || board.LevelScene == SceneType::MoonNight)
		return row * 85 + 40 + max(0, 5 - col) * 15;
	return 0;
}

byte __asm__DrawImage[35]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	MOV_EBX_PTR_ADDR(0),
	MOV_EAX(0),
	INVOKE(0x587150),
	RET
};

void DrawImage(int x, int y, int GraphicsID, int ImageAddr)
{
	SETARG(__asm__DrawImage, 1) = y;
	SETARG(__asm__DrawImage, 6) = x;
	SETARG(__asm__DrawImage, 12) = ImageAddr;
	SETARG(__asm__DrawImage, 17) = GraphicsID;
	Memory::Execute(STRING(__asm__DrawImage));
}

byte __asm__TodDrawImageScaledF[48]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	PUSHDWORD(0),
	0x8B,0x05,INUMBER(0),//mov eax,[d]
	MOV_ECX(0),
	INVOKE(0x512950),
	ADD_ESP(0x10),
	RET
};

void TodDrawImageScaledF(float scale_x, float scale_y, float x, float y, int GraphicsID, int ImageAddr)
{
	SETARGFLOAT(__asm__TodDrawImageScaledF, 1) = scale_y;
	SETARGFLOAT(__asm__TodDrawImageScaledF, 6) = scale_x;
	SETARGFLOAT(__asm__TodDrawImageScaledF, 11) = y;
	SETARGFLOAT(__asm__TodDrawImageScaledF, 16) = x;
	SETARG(__asm__TodDrawImageScaledF, 22) = ImageAddr;
	SETARG(__asm__TodDrawImageScaledF, 27) = GraphicsID;
	Memory::Execute(STRING(__asm__TodDrawImageScaledF));
}

void onBoardDrawImage(int GraphicsID, MyBoard board)
{
	auto plants = board.GetAllPlants<MyPlant>();
	for (auto& plant : plants)
	{
		int row = plant.Row, col = plant.Column, hp = plant.Hp, max_hp = plant.MaxHp;
		float hp_ratio = (float)hp / (float)max_hp;
		int x = GridToPixelX(row, col, board), y = GridToPixelY(row, col, board);
		int ix=0, iy=0;
		//绘制血条
		if (plant.HpDisplayCounter>0 || plant.EatenCounter>0 || hp_ratio<0.33f)//这里被啃50cs应该可以改一改?
		{
			ix = x + 9;
			iy = y + 100;
			DrawImage(ix, iy, GraphicsID, 0x6FF0A0);
			TodDrawImageScaledF(hp_ratio, 1.0f, (float)ix, (float)iy, GraphicsID, 0x6FF09C);
			DrawImage(ix, iy, GraphicsID, 0x6FF0A4);
		}
		//绘制等级图标
		ix = x - 20;
		iy = y + 95;
		DrawImage(ix, iy, GraphicsID, 0x6FF084 + 4 * plant.Level);
	}
}

void InitBoardEvents()
{
	PVZEvent::BoardInitAfterEvent((int)onBoardInit);
	UpdateGameObjectsEvent((int)onBoardUpdateGameObject);
	PVZEvent::BoardDrawImageEvent((int)onBoardDrawImage);
}