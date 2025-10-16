#include "pch.h"
#include "MyPlant/PlantAbility.hpp"

void InitPlantExistCount(MyBoard& board)
{
	std::fill(MyBoard::GoldMagnetFactors.begin(), MyBoard::GoldMagnetFactors.end(), 1.0f);
	std::fill(MyBoard::IceShroomCounts.begin(), MyBoard::IceShroomCounts.end(), false);
}

void onBoardInit(MyBoard board)
{
	board.GetPVZApp().FreePlantingCheat = true;

	board.MatchRunning = false;
	board.MatchTimer = 0;

	board.PoisonCounter = 1;
	board.ZombieColorShiftCounter = 1;
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
			if (zombie.Type == ZombieType::NewspaperZombie && zombie.FromWave >= WAVE_ELITE_MASK)
				continue;
			zombie.Hit(zombie.PoisonStack, PVZ::DAMAGEF_NOFLASH);
		}
	}
	board.ZombieColorShiftCounter++;
	if (board.ZombieColorShiftCounter == 100)
	{
		board.ZombieColorShiftCounter = 0;
		auto zombies = board.GetAllZombies<MyZombie>();
		for (auto& zombie : zombies)
		{
			switch (zombie.ColorFlag)
			{
			case 1:
				{
				if (zombie.FrostStack)
					zombie.ColorFlag = 2;
				else if (zombie.FlameStack)
					zombie.ColorFlag = 3;
				break;
				}
			case 2:
				{
				if (zombie.FlameStack)
					zombie.ColorFlag = 3;
				else if (zombie.PoisonStack)
					zombie.ColorFlag = 1;
				break;
				}
			case 3:
				{
				if (zombie.PoisonStack)
					zombie.ColorFlag = 1;
				else if (zombie.FrostStack)
					zombie.ColorFlag = 2;
				break;
				}
			default:
				{
				if (zombie.PoisonStack)
					zombie.ColorFlag = 1;
				else if (zombie.FrostStack)
					zombie.ColorFlag = 2;
				else if (zombie.FlameStack)
					zombie.ColorFlag = 3;
				else
					zombie.ColorFlag = 0;
				break;
				}
			}
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

void UpdateMatch(MyBoard& board)
{
	board.MatchTimer += 1;
	if (board.MatchTimer % 500 == 10)
	{
		for (int row = 0; row < 5; row++)
		{
			board.AddZombieInRow(ZombieType::ConeheadZombie, row, 0);
		}
	}
}

void onBoardUpdateGameObject(MyBoard board)
{
	UpdatePoisonApply(board);
	UpdatePlantExistCount(board);
	
	/*
	auto plants = board.GetAllPlants<MyPlant>();
	for (auto& plant : plants)
		if (plant.OnBoard && !plant.Squash && !plant.Sleeping && plant.mOnBungee == 0)
			PlantAbility::GetAbility(plant.Type)->TickPassive(plant);
	*/

	if (board.MatchRunning)
		UpdateMatch(board);
}

byte __asm__MyDrawImage[35]
{
	PUSHDWORD(0),
	PUSHDWORD(0),
	MOV_EBX_PTR_ADDR(0),
	MOV_EAX(0),
	INVOKE(0x587150),
	RET
};

void MyDrawImage(int x, int y, int GraphicsID, int ImageAddr)
{
	SETARG(__asm__MyDrawImage, 1) = y;
	SETARG(__asm__MyDrawImage, 6) = x;
	SETARG(__asm__MyDrawImage, 12) = ImageAddr;
	SETARG(__asm__MyDrawImage, 17) = GraphicsID;
	Memory::Execute(STRING(__asm__MyDrawImage));
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

byte __asm__MyDrawString[71]
{
	MOV_ECX_PTR_ADDR(0x6A74B0),//字体地址 Font*
	MOV_EAX(0),//Graphics*
	MOV_PTR_EUX_ADD_V_EVX(REG_EAX,REG_ECX,0x40),
	MOV_PTR_EUX_ADD_V(REG_EAX,0x30,0xFF),//R
	MOV_PTR_EUX_ADD_V(REG_EAX,0x34,0xFF),//G
	MOV_PTR_EUX_ADD_V(REG_EAX,0x38,0xFF),//B
	MOV_PTR_EUX_ADD_V(REG_EAX,0x3C,0xFF),//A
	PUSHDWORD(0),//Y
	PUSHDWORD(0),//X
	PUSHDWORD(0),//string&
	INVOKE(0x587120),
	RET
};
//这个函数暂时有问题，原因未知
void MyDrawString(int x, int y, const std::string& string, int GraphicsAddr)
{
	SETARG(__asm__MyDrawString, 7) = GraphicsAddr;
	SETARG(__asm__MyDrawString, 43) = y;
	SETARG(__asm__MyDrawString, 48) = x;
	SETARG(__asm__MyDrawString, 53) = (unsigned int)&string;
	Memory::Execute(STRING(__asm__MyDrawString));
}


void onBoardDrawImage(int GraphicsID, MyBoard board)
{
	//DrawString(200, 200, "啊啊啊啊", GraphicsID);

	auto plants = board.GetAllPlants<MyPlant>();
	for (auto& plant : plants)
	{
		if (plant.OwnerID == 0)
		{
			int row = plant.Row, col = plant.Column, hp = plant.Hp, max_hp = plant.MaxHp;
			float hp_ratio = (float)hp / (float)max_hp;

			//int x = board.GridToXPixel(row, col), y = board.GridToYPixel(row, col);
			int x = plant.ImageX, y = plant.ImageY;

			int ix = 0, iy = 0;
			//绘制血条
			if (plant.HpDisplayCounter > 0 || hp_ratio < 0.33f)
			{
				ix = x + 9;
				iy = y + 60;
				MyDrawImage(ix, iy, GraphicsID, 0x6FF0A0);
				TodDrawImageScaledF(hp_ratio, 1.0f, (float)ix, (float)iy, GraphicsID, 0x6FF09C);
				MyDrawImage(ix, iy, GraphicsID, 0x6FF0A4);
			}
			//绘制等级图标
			ix = x - 20;
			iy = y + 55;
			MyDrawImage(ix, iy, GraphicsID, 0x6FF084 + 4 * plant.Level);
		}
	}
}

void onTyping(MyBoard board, char key)
{
	auto app = board.GetPVZApp();
	if (app.LevelId == PVZLevel::Survival_Night)
	{
		switch (key)
		{
		case 'P':
			board.MatchRunning = !board.MatchRunning;
			break;
		case 'F':
		{
			auto plants = board.GetAllPlants<MyPlant>();
			for (auto& plant : plants)
			{
				plant.AddExperience(100000);
			}
			break;
		}
		default:
			break;
		}
	}
}

bool onBoardCallPlantUpdate(MyPlant plant)
{
	//血条倒计时
	if (plant.HpDisplayCounter > 0)
		plant.HpDisplayCounter -= 1;

	//被动技能，无视减速等效果
	if (plant.OnBoard && !plant.Squash && !plant.Sleeping && plant.mOnBungee == 0)
		PlantAbility::GetAbility(plant.Type)->TickPassive(plant);

	//肥料加速100%
	if (plant.FertilizedCounter > 0)
	{
		plant.FertilizedCounter -= 1;
		PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EAX, plant.GetBaseAddress()).invoke(0x463E40).ret());
	}
	return true;
}

void InitBoardEvents()
{
	MyBoard::SetMemSize(0x6000);
	PVZEvent::BoardInitAfterEvent((int)onBoardInit);
	UpdateGameObjectsEvent((int)onBoardUpdateGameObject);
	PVZEvent::BoardCallPlantUpdateEvent((int)onBoardCallPlantUpdate);
	PVZEvent::BoardDrawImageEvent((int)onBoardDrawImage);
	PVZEvent::TypingEvent((int)onTyping);

	//所有关卡暂停出怪
	PVZ::Memory::WriteMemory<byte>(0x413E4B, 0x00);
	//白天关卡暂停天降阳光
	PVZ::Memory::WriteMemory<byte>(0x413B82, 0x00);
}