#include "pch.h"
#include "MyPlant/PlantAbility.hpp"

void InitPlantExistCount(MyBoard& board)
{
	std::fill(MyBoard::GoldMagnetFactors.begin(), MyBoard::GoldMagnetFactors.end(), 1.0f);
	std::fill(MyBoard::IceShroomCounts.begin(), MyBoard::IceShroomCounts.end(), false);
	std::fill(MyBoard::BloverAccelerateCounts.begin(), MyBoard::BloverAccelerateCounts.end(), 0);
}

void onBoardInit(MyBoard board)
{
	board.MatchRunning = false;
	board.MatchTimer = 0;

	board.EnablePlantAbility = true;

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
			zombie.ShiftColorFlag();
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

		switch (type)
		{
		case SeedType::GoldMagnet:
			{
				switch (level)
				{
				case 0:
				case 1:
					MyBoard::GoldMagnetFactors[row] *= 1.5f;
					break;
				case 2:
				case 3:
					MyBoard::GoldMagnetFactors[row] *= 1.75f;
					break;
				case 4:
				case 5:
					MyBoard::GoldMagnetFactors[row] *= 2.0f;
					break;
				default:
					break;
				}
			}
			break;
		case SeedType::Iceshroom:
			MyBoard::IceShroomCounts[row] = true;
			break;
		case SeedType::Blover:
			{
				if (plant.BloverIsWorking && plant.AttributeCountdown == 0)
				{
					MyBoard::BloverAccelerateCounts[row] += 1;
					plant.AttributeCountdown = (plant.BloverIsFevering ? 2 : 4);
				}
			}
			break;
		default:
			break;
		}
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
	board.Timer10cs++;
	if (board.Timer10cs == 10)
		board.Timer10cs = 0;

	board.Timer1s++;
	if (board.Timer1s == 100)
		board.Timer1s = 0;

	UpdatePoisonApply(board);
	UpdatePlantExistCount(board);
	
	/*
	* 已被移至 onBoardCallPlantUpdate
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

/// @deprecated 后续代码请使用该函数的重载。
void MyDrawImage(int x, int y, int GraphicsID, DWORD ImageAddr)
{
	SETARG(__asm__MyDrawImage, 1) = y;
	SETARG(__asm__MyDrawImage, 6) = x;
	SETARG(__asm__MyDrawImage, 12) = ImageAddr;
	SETARG(__asm__MyDrawImage, 17) = GraphicsID;
	Memory::Execute(STRING(__asm__MyDrawImage));
}

void MyDrawImage(int x, int y, int Graphics, MyImage image)
{
	PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(y).push_imm32(x)
		.mov_reg_imm(REG_EAX, Graphics)
		.mov_reg_imm(REG_EBX, image.GetBaseAddress())
		.invoke(0x587150)
		.ret()
	);
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
/// @deprecated
void MyDrawString(int x, int y, const std::string& string, int GraphicsAddr)
{
	SETARG(__asm__MyDrawString, 7) = GraphicsAddr;
	SETARG(__asm__MyDrawString, 43) = y;
	SETARG(__asm__MyDrawString, 48) = x;
	SETARG(__asm__MyDrawString, 53) = (unsigned int)&string;
	Memory::Execute(STRING(__asm__MyDrawString));
}

void MyDrawString(int x, int y, const char* text, int Graphics)
{
	auto str = Draw::ToString(text);
	Draw::DrawString(x, y, str, Graphics);
	//销毁字符串
	DestroyPString(str);
}


void onBoardDrawImage(int Graphics, MyBoard board)
{
	//拷贝一份Graphics用于新绘制
	int new_graphics = PVZ::Memory::Execute(AsmBuilder()
		.push_imm32(Graphics)
		.invoke(0x586C30)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
	//设置字体
	PVZ::Memory::WriteMemory<DWORD>(new_graphics + 0x40, PVZ::Memory::ReadMemory<DWORD>(0x6A74B0));

	MyDrawString(200, 300, "120English Chinese\xD1\xAA\xCC\xF5\xB5\xD7", new_graphics);

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
			if (plant.Type==SeedType::Blover || plant.HpDisplayCounter > 0 || hp_ratio < 0.33f)
			{
				ix = x + 9;
				iy = y + 60;
				//MyDrawImage(ix, iy, Graphics, (DWORD)0x6FF0A0);
				MyDrawImage(ix, iy, new_graphics, NewImage::HP_BAR_BASE);
				TodDrawImageScaledF(hp_ratio, 1.0f, (float)ix, (float)iy, Graphics, 0x6FF09C);
				//MyDrawImage(ix, iy, Graphics, (DWORD)0x6FF0A4);
				MyDrawImage(ix, iy, new_graphics, NewImage::HP_BAR_BORDER);
			}
			//绘制等级图标
			ix = x - 20;
			iy = y + 55;
			//MyDrawImage(ix, iy, Graphics, (DWORD)(0x6FF084 + 4 * plant.Level));
			MyDrawImage(ix, iy, new_graphics, NewImage::PLANT_LEVELS[plant.Level]);
		}
	}
	//释放Graphics
	PVZ::Memory::Execute(AsmBuilder()
		.mov_reg_imm(REG_ECX, new_graphics)
		.invoke(0x586B10)
		.ret()
	);
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
		case 'S':
			board.EnablePlantAbility = !board.EnablePlantAbility;
			if (board.EnablePlantAbility)
				Creator::CreateCaption(STRING("Plant abilities switched on"), CaptionStyle::Bottom,100);
			else
				Creator::CreateCaption(STRING("Plant abilities switched off"), CaptionStyle::Bottom,100);
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

inline void UpdatePlant(MyPlant& plant)
{
	PVZ::Memory::Execute(AsmBuilder().mov_reg_imm(REG_EAX, plant.GetBaseAddress()).invoke(0x463E40).ret());
}

bool onBoardCallPlantUpdate(MyPlant plant)
{
	//血条倒计时
	if (plant.HpDisplayCounter > 0)
		plant.HpDisplayCounter -= 1;
	//晕眩倒计时
	if (plant.StunCountdown > 0)
		plant.StunCountdown -= 1;

	//被动技能，无视减速等效果
	if (plant.OnBoard && !plant.Squash && !plant.Sleeping && plant.mOnBungee == 0)
	{
		auto plant_prototype = PlantAbility::GetAbility(plant.Type);
		MyBoard board = plant.GetBoard();
		if (board.Timer1s == 0)
		{
			//自回复
			plant_prototype->SelfHeal(plant);
		}
		//使用被动技能
		plant_prototype->TickPassive(plant);

		//三叶草加速除了三叶草之外的植物
		if (plant.Type != SeedType::Blover)
		{
			int time = MyBoard::BloverAccelerateCounts[plant.Row];
			while (time > 0)
			{
				time--;
				UpdatePlant(plant);
			}
		}
		//肥料加速100%
		if (plant.FertilizedCounter > 0)
		{
			plant.FertilizedCounter -= 1;
			UpdatePlant(plant);
		}
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
	//自由种植
	PVZ::Memory::WriteMemory<byte>(0x40FE30, 0x81);
}