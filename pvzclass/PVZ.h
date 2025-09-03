#pragma once
#include "Enums.h"
#include "Flags.h"
#include "AsmFunctions.h"
#include "AsmBuilder.hpp"
#include "Memory.hpp"
#include <ctime>
#include <memory>
#include <vector>
#include <type_traits>

using std::enable_if_t;
using std::is_base_of;

#pragma region definitions

#define STRING(str) str,sizeof(str)/sizeof(*str)
#define SETARG(asmfunction,index) *(int*)(asmfunction+index)
#define SETARGFLOAT(asmfunction,index) *(float*)(asmfunction+index)

#define LOGICALINCLUDE(c,v) (c&v)==v

#define SPT std::shared_ptr
#define UPT std::unique_ptr
#define MKU std::make_unique
#define MKS std::make_shared

#pragma endregion

#define AP_ZOMBIESPEED		1
#define AP_ZOMBIECOLOR		2
#define AP_ZOMBIESCALE		4
#define AP_PLANTCOLOR		8
#define AP_PLANTSCALE		16

#define GPD_RIGHT			0
#define GPD_LEFT			1

#define MINGAM_ENABLE		2
#define MINGAM_DISABLE		-1

#define MINGAME_STARTED		1
#define MINGAME_NOTSTARTED	-1

#define APA_LOOP			1
#define APA_ONCE_DISAPPEAR	2
#define APA_ONCE_STOP		3

#define ID_INDEX(id) ((id) & 0x0000FFFF)
#define ID_RANK(id) ((id) & 0xFFFF0000)

/// @deprecated
#define HZC_COMMON 1
/// @deprecated
#define HZC_BALLOON_AIR 2
/// @deprecated
#define HZC_SNORKED_UNDER 4
/// @deprecated
#define HZC_NOT_GROUNDED 16
/// @deprecated
#define HZC_DYING 32
/// @deprecated
#define HZC_DIGGER_UNDER 64
/// @deprecated
#define HZC_HYPNOTIZED 128

#define INVALID_BASEADDRESS 0x400000

/// @brief 包含大部分用于控制 PVZ 内部对象的类和方法。
/// @note Only version 1.0.0.1051 is fully supported
namespace PVZ
{
	/// @brief 初始化 PVZ 命名空间，且不在程序内附加钩子。Memory::immediateExecute 会设置为 true 。
	/// @param pid 进程 id
	void InitPVZNoLock(DWORD pid);

	void InitPVZ(DWORD pid);
	void QuitPVZ();

#pragma region structs

	struct Color
	{
		int Red;
		int Green;
		int Blue;
		int Alpha;
	};

#pragma endregion

#pragma region classes	
	/// @brief 所有对应 PVZ 内部对象的类的基类。
	/// @attention 你不应该直接构造 BaseClass！
	class BaseClass
	{
	protected:
		/// @brief 对应对象的基地址
		int BaseAddress;
	public:
		BaseClass() : BaseAddress(INVALID_BASEADDRESS) {};
		BaseClass(int address) : BaseAddress(address){};
		/// @brief 返回基址
		/// @return 基址
		int GetBaseAddress() const
		{ return(this->BaseAddress); }
		/// @brief 对应的对象是否已经失效，或者构造不良。
		/// @return 是否已经失效或构造不良。
		const bool isValid()
		{ return(this->BaseAddress != INVALID_BASEADDRESS && this->BaseAddress != 0); }
	};

	class Rect
	{
	public:
		int X;
		int Y;
		int Width;
		int Height;
		// 判定坐标为 (X, Y)，半径为 radius 的圆与该矩阵是否有重叠部分。
		// 相切会视为有重叠部分。
		bool IsCircleOverlap(const int X, const int Y, const int radius);
	};
	// 取得两个矩形横向重叠部分的长度。
	// 若横向无重叠部分，返回两矩形横向间距的相反数。
	/// @param rect 计算重叠的另一个矩形。
	/// @return 矩形横向重叠的长度，或矩形横向间距的相反数。
	int GetXOverlap(const Rect& rect1, const Rect& rect2);

	typedef Rect CollisionBox;

	class PVZutil
	{
	public:
		/// @brief 获取 PVZClass 版本
		READONLY_PROPERTY(const char*,				__get_Version)		Version;
		/// @brief 判断 PVZ 主程序是哪一个发布版本。
		/// @see PVZVersion
		READONLY_PROPERTY(PVZVersion::PVZVersion,	__get_GameVersion)	GameVersion;
	};

	/// @brief 游戏程序主类（原 LawnApp）。
	class PVZApp : public BaseClass
	{
	public:
		PVZApp(DWORD address) : BaseClass(address) {};
		/// @brief 音乐音量大小，数值应当在 [0,1] 范围内。
		PROPERTY(double,						__get_MusicVolume,	__set_MusicVolume)			aMusicVolume;
		/// @brief 音效音量大小，数值应当在 [0,1] 范围内。
		PROPERTY(double,						__get_SoundFXVolume,__set_SoundFXVolume)		SoundFXVolume;
		/// @brief 主线程 ID
		T_PROPERTY(DWORD,						PrimaryThreadId,	__get_PrimaryThreadId,		__set_PrimaryThreadId,		0x33C);
		/// @brief 是否关闭
		T_PROPERTY(BOOLEAN,						Shutdown,			__get_Shutdown,				__set_Shutdown,				0x341);
		/// @brief 窗口句柄
		T_PROPERTY(HWND,						HWnd,				__get_HWnd,					__set_HWnd,					0x350);
		/// @brief 当前模式类型
		T_PROPERTY(PVZLevel::PVZLevel,			LevelId,			__get_LevelId,				__set_LevelId,				0x7F8);
		/// @brief 游戏状态
		/// @see PVZGameState
		T_PROPERTY(PVZGameState::PVZGameState,	GameState,			__get_GameState,			__set_GameState,			0x7FC);
		/// @brief 是否开启自由种植
		T_PROPERTY(BOOLEAN,						FreePlantingCheat,	__get_FreePlantingCheat,	__set_FreePlantingCheat,	0x814);
		/// @brief 是否在玩完整版。部分 PVZ 主程序反转了它的效果。
		T_PROPERTY(BOOLEAN,						FullVersion,		__get_FullVersion,			__set_FullVersion,			0x8C0);
	};
	/// @brief 获取游戏程序主对象
	/// @return 游戏程序主对象
	PVZApp GetPVZApp();

	/// @brief 图片类
	class Image : public BaseClass
	{
	public:
		Image(int address) : BaseClass(address){};
	};

	class Attachment;
	class AttachmentID;
	class Zombie;
	class Plant;
	class Projectile;
	class Coin;
	class LawnMower;
	class Griditem;
	class MousePointer;
	class Caption;
	class CardSlot;
	class Challenge;
	class Lawn;
	class Icetrace;
	class Wave;
	/// @brief 控件
	class Widget : public BaseClass
	{
	public:
		Widget(int address) : BaseClass(address) {};
		INT_PROPERTY(ViewX,			__get_ViewX,		__set_ViewX,		0x30);
		INT_PROPERTY(ViewY,			__get_ViewY,		__set_ViewY,		0x34);
		INT_PROPERTY(ViewLength,	__get_ViewLength,	__set_ViewLength,	0x38);
		INT_PROPERTY(ViewHeight,	__get_ViewHeight,	__set_ViewHeight,	0x3C);
	};
	/// @brief 三行三列矩阵
	class Matrix3 : public BaseClass
	{
	public:
		Matrix3(DWORD address) : BaseClass(address) {};

		/// @brief 根据指定的平移坐标、旋转弧度和拉伸比例，设定矩阵每个项的数值。
		void ScaleRotateTransformMatrix(float x, float y, float rad, float ScaleX, float ScaleY);
	};
	/// @brief 包含大部分关卡内部数据和对象的类	
	class Board : public Widget
	{
	protected:
		/// @brief 所有 GetAll() 形式函数的原型，获取 DataArray\<T\> 中的全体成员。
		/// @tparam T 成员类型
		/// @note T 必须具有 MemSize 静态常量，且类型为整数
		/// @tparam _Base_offset 基址的偏移量
		/// @tparam _Max_offset 最大数量的偏移量
		/// @tparam _T_Dead_offset 在 T 中，表示该成员已被移除变量的偏移量。此变量视为用 byte 存储。
		/// @return 装有全体 T 成员对象的 std::vector
		template<typename T, size_t _Base_offset, size_t _Max_offset, size_t _T_Dead_offset>
		std::vector<T> __prototype_GetAll()
		{
			std::vector<T> container;
			int maxnum = Memory::ReadMemory<int>(BaseAddress + _Max_offset);
			DWORD base_addr = Memory::ReadMemory<DWORD>(BaseAddress + _Base_offset);
			for (int i = 0; i < maxnum; i++)
			{
				if (!Memory::ReadMemory<byte>(base_addr + _T_Dead_offset + T::MemSize * i))
					container.push_back(T(i));
			}
			return container;
		}
	public:
		Board(int address) : Widget(address) {};

		/// @brief 调整该类在 PVZ 中对象的大小。
		/// @note 请在派生类中调用这个函数。
		/// @note 调用该函数后，新生成的存档与原版存档不兼容，请注意清理。
		/// @note 额外的空间未经初始化，使用前请设法初始化。
		/// @param MemSize 更改后的大小。
		static void SetMemSize(int NewSize);

		PVZApp GetPVZApp();
		/// @brief 场上僵尸数量
		INT_READONLY_PROPERTY(ZombiesCount, __get_ZombiesCount, 0xA0);
		/// @brief 场上植物数量
		INT_READONLY_PROPERTY(PlantsCount, __get_PlantsCount, 0xBC);
		/// @brief 场上子弹数量
		INT_READONLY_PROPERTY(ProjectilesCount, __get_ProjectilesCount, 0xD8);
		/// @brief 场上掉落物数量
		INT_READONLY_PROPERTY(CoinsCount, __get_CoinsCount, 0xF4);
		/// @brief 场上小推车数量
		INT_READONLY_PROPERTY(LawnmoversCount, __get_LawnmoversCount, 0x110);
		/// @brief 场地物品总数
		INT_READONLY_PROPERTY(GriditemsCount, __get_GriditemsCount, 0x12C);
		/// @brief 游戏是否已暂停
		T_PROPERTY(BOOLEAN, GamePaused, __get_GamePaused, __set_GamePaused, 0x164);

#pragma region fog

		/// @brief 获得指定格的雾的浓度
		/// @param row 行
		/// @param column 列
		/// @return 雾的浓度
		int GetGridFog(int row, int column);
		/// @brief 雾的偏移
		T_PROPERTY(FLOAT, FogOffset, __get_FogOffset, __set_FogOffset, 0x5D0);
		/// @brief 雾吹飞效果倒计时
		INT_PROPERTY(FogBlownCountDown, __get_FogBlownCountDown, __set_FogBlownCountDown, 0x5D4);

#pragma endregion

		/// @brief 天降阳光倒计时
		INT_PROPERTY(SunDropCountdown, __get_SunDropCountdown, __set_SunDropCountdown, 0x5538);
		/// @brief 天降阳光数
		INT_PROPERTY(SunDropCount, __get_SunDropCount, __set_SunDropCount, 0x553C);
		/// @brief 场景类型
		PROPERTY(SceneType::SceneType, __get_LevelScene, __set_LevelScene) LevelScene;
		/// @brief 冒险模式关卡
		INT_PROPERTY(AdventureLevel, __get_AdventureLevel, __set_AdventureLevel, 0x5550);
		/// @brief 当前阳光数
		INT_PROPERTY(Sun, __get_Sun, __set_Sun, 0x5560);
		/// @brief 总波数
		PROPERTY(int, __get_WaveCount, __set_WaveCount) WaveCount;
		/// @brief 关卡主更新次数
		INT_PROPERTY(PlayingTime, __get_PlayingTime, __set_PlayingTime, 0x5568);
		/*include preparing time*/
		INT_READONLY_PROPERTY(PlayingTime2, __get_PlayingTime2, 0x556C);
		/*lose focus and recount*/
		INT_READONLY_PROPERTY(PlayingTime3, __get_PlayingTime3, 0x5570);
		/// @brief 当前波数
		INT_READONLY_PROPERTY(CurrentWave, __get_CurrentWave, 0x557C);
		/// @brief 已刷新波数
		INT_READONLY_PROPERTY(RefreshedWave, __get_RefreshedWave, 0x5580);
		/// @brief 教程状态
		INT_PROPERTY(FlashTip, __get_FlashTip, __set_FlashTip, 0x5584);
		/*Flash tips for novice tutorials*/
		INT_PROPERTY(RefreshHp, __get_RefreshHp, __set_RefreshHp, 0x5594);
		INT_READONLY_PROPERTY(CurrentWaveHp, __get_CurrentWaveHp, 0x5598);
		/// @brief 下一波倒计时
		INT_PROPERTY(NextWaveCountdown, __get_NextWaveCountdown, __set_NextWaveCountdown, 0x559C);
		/// @brief 下一波倒计时的初值
		INT_READONLY_PROPERTY(NextWaveCountdownInitialValue, __get_NextWaveCountdownInitialValue, 0x55A0);
		/// @brief 一大波僵尸的倒计时
		INT_PROPERTY(HugeWaveCountdown, __get_HugeWaveCountdown, __set_HugeWaveCountdown, 0x55A4);
		/// @brief 是否显示铲子
		T_PROPERTY(BOOLEAN, HaveShovel, __get_HaveShovel, __set_HaveShovel, 0x55F1);
		/// @brief 金钱数显示消失倒计时
		INT_PROPERTY(ShowMoneyCountdown, __get_ShowMoneyCountdown, __set_ShowMoneyCountdown, 0x55F4);
		/// @brief 调试模式
		T_PROPERTY(DebugModeType::DebugModeType, DebugMode, __get_DebugMode, __set_DebugMode, 0x55F8);
		/// @brief 关卡进度条
		INT_PROPERTY(LevelProcessBar, __get_LevelProcessBar, __set_LevelProcessBar, 0x5610);
		/// @brief 是否激活 Mustatche
		T_PROPERTY(BOOLEAN, Mustache, __get_Mustache, __set_Mustache, 0x5761);
		/// @brief 是否激活 Trickedout
		T_PROPERTY(BOOLEAN, Trickedout, __get_Trickedout, __set_Trickedout, 0x5762);
		/// @brief 是否激活 Future
		T_PROPERTY(BOOLEAN, Future, __get_Future, __set_Future, 0x5763);
		/// @brief 是否激活 Pinata
		T_PROPERTY(BOOLEAN, Pinata, __get_Pinata, __set_Pinata, 0x5764);
		/// @brief 是否激活 Dance
		T_PROPERTY(BOOLEAN, Dance, __get_Dance, __set_Dance, 0x5765);
		/// @brief 是否激活 Daisies
		T_PROPERTY(BOOLEAN, Daisies, __get_Daisies, __set_Daisies, 0x5766);
		/// @brief 是否激活 Sukhbir
		T_PROPERTY(BOOLEAN, Sukhbir, __get_Sukhbir, __set_Sukhbir, 0x5767);
		/// @brief 被吃掉的植物总数
		INT_READONLY_PROPERTY(EatenPlants, __get_EatenPlants, 0x5798);
		/// @brief 被铲除的植物总数
		INT_READONLY_PROPERTY(ShoveledPlants, __get_ShoveledPlants, 0x579C);
		
		/// @brief 获取出怪类型
		/// @param ztypes 返回值存放位置
		void GetZombieAllowed(ZombieType::ZombieType* ztypes);

		/// @brief 判断当前场景是否为六行场地
		READONLY_PROPERTY_BINDING(
			BOOLEAN,
			__get_SixRoute,
			(LevelScene == SceneType::Pool) || (LevelScene == SceneType::Fog)) SixRoute;

#pragma region methods

		/// @brief 将列数转换为 X 坐标
		/// @param row 行
		/// @param column 列
		/// @return 对应 X 坐标
		int GridToXPixel(int row, int column);
		/// @brief 将行数转换为 Y 坐标
		/// @param row 行
		/// @param column 列
		/// @return 对应 Y 坐标
		int GridToYPixel(int row, int column);
		/// @brief 设置突袭（墓碑刷怪、水下僵尸、蹦极空降）僵尸的倒计时
		/// @param countdown 设置的倒计时，默认为 0.01 秒。
		void Assault(int countdown = 1);
		/// @brief 设置最后一波音效倒计时
		/// @param countdown 设置的倒计时，默认为 0.01 秒。
		void Bell(int countdown = 1);
		/// @brief 震动画面面板
		/// @param horizontalAmplitude X 轴方向的震动大小
		/// @param verticalAmplitude 
		/// @param duration 震动持续时间
		void Earthquake(int horizontalAmplitude = 2, int verticalAmplitude = 4, int duration = 20);
		/// @brief 立刻失败。
		void Lose();
		/// @brief 若当前可以承担 amount 点阳光的支出，则消耗 theAmount 阳光，
		//	否则触发阳光数量不足的的音效和闪红特效。
		/// @param amount 阳光消耗数值。
		/// @return 是否可以承担支出。
		bool TakeSunMoney(int amount);
		/// @brief 立刻获胜。
		void Win();
		/// @brief 保存游戏存档。
		/// @param path 存档路径
		/// @param pathlen path 的长度
		/// @return 是否保存成功。
		bool Save(const char* path, int pathlen);
		/// @brief 读取游戏存档。
		/// @param path 存档路径
		/// @param pathlen path 的长度
		/// @return 是否载入成功
		bool Load(const char* path, int pathlen);

#pragma endregion

#pragma region getmethod
		/// @brief 获取 DataArray\<Zombie\> 中的全体对象。
		/// @tparam T 成员的类型，必须为 Zombie 或它的派生类。
		/// @return 装有全体 Zombie （或者其派生类）对象的 std::vector
		template<typename T = Zombie, typename = enable_if_t<is_base_of<Zombie, T>::value>>
		std::vector<T> GetAllZombies()
		{
			return __prototype_GetAll<T, 0x90, 0x94, 0x0EC>();
		}
		/// @brief 获取 DataArray\<Plant\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 Plant 或它的派生类。
		/// @return 装有全体 Plant （或者其派生类）对象的 std::vector
		template<typename T = Plant, typename = enable_if_t<is_base_of<Plant, T>::value>>
		std::vector<T> GetAllPlants()
		{
			return __prototype_GetAll<T, 0x0AC, 0x0B0, 0x141>();
		}
		/// @brief 获取 DataArray\<Projectile\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 Projectile 或它的派生类。
		/// @return 装有全体 Projectile （或者其派生类）对象的 std::vector
		template<typename T = Projectile, typename = enable_if_t<is_base_of<Projectile, T>::value>>
		std::vector<T> GetAllProjectile()
		{
			return __prototype_GetAll<T, 0x0C8, 0x0CC, 0x50>();
		}
		/// @brief 获取 DataArray\<Coin\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 Coin 或它的派生类。
		/// @return 装有全体 Coin （或者其派生类）对象的 std::vector
		template<typename T = Coin, typename = enable_if_t<is_base_of<Coin, T>::value>>
		std::vector<T> GetAllCoins()
		{
			return __prototype_GetAll<T, 0x0E4, 0x0E8, 0x38>();
		}
		/// @brief 获取 DataArray\<LawnMower\> 中的全体对象。
		/// @tparam T 成员值的类型，必须为 LawnMower 或它的派生类。
		/// @return 装有全体 LawnMower （或者其派生类）对象的 std::vector
		template<typename T = LawnMower, typename = enable_if_t<is_base_of<LawnMower, T>::value>>
		std::vector<T> GetAllLawnmovers()
		{
			return __prototype_GetAll<T, 0x100, 0x104, 0x30>();
		}
		/// @brief 获取 DataArray\<Griditem\> 中的全体对象。
		/// @note 与其他 GetAll() 不同，此函数不依赖于 __prototype_GetAll() 。
		/// @note 类型不为 T 的 Griditem 会被滤去。
		/// @tparam T 成员值的类型，必须为 Griditem 或它的派生类。
		/// @return 装有全体 Griditem （或者其派生类）对象的 std::vector
		template<typename T = Griditem, typename = enable_if_t<is_base_of<Griditem, T>::value>>
		std::vector<T> GetAllGriditems()
		{
			std::vector<T> griditems;
			int maxnum = Memory::ReadMemory<int>(BaseAddress + 0x120);
			DWORD base_addr = Memory::ReadMemory<DWORD>(BaseAddress + 0x11C);
			for (int i = 0; i < maxnum; i++)
			{
				if (!Memory::ReadMemory<byte>(base_addr + 0x20 + T::MemSize * i)
					&& (T::ItemType == 0 || Memory::ReadMemory<byte>(base_addr + 8 + T::MemSize * i) == T::ItemType))
						griditems.push_back(T(i));
			}
			return griditems;
		}
		Lawn GetLawn();
		Icetrace GetIcetrace();
		Wave GetWave(int index);
		MousePointer GetMousePointer();
		Caption GetCaption();
		CardSlot GetCardSlot();
		/// @brief 获取 Challenge 类型的成员。
		/// @tparam T 返回值的类型，必须为 Challenge 或它的派生类。
		/// @return Challenge （或者其派生类）成员对象 
		template<typename T = Challenge, typename = enable_if_t<is_base_of<Challenge, T>::value>>
		T GetChallenge()
		{
			return T(BaseAddress);
		}
		/// @brief 获取 Challenge 类型的成员。
		/// @attention 与 GetChallenge() 不同，此方法只能获得 Challenge 类型的对象。
		/// @return Challenge 成员对象 
		Challenge GetMiscellaneous();
#pragma endregion
	};
	/// @brief 部分类的基类
	/// @attention 你不应该直接构造这个类！
	class GameObject : public BaseClass
	{
	public:
		GameObject() : BaseClass(0) {};
		/// @brief 获取 GameObject 所在的 PVZApp
		PVZApp GetLawnApp()
		{ return(PVZ::PVZApp(Memory::ReadMemory<DWORD>(BaseAddress))); }
		/// @brief 获取当前对象所属的 Board
		/// @return 对象所在的 Board
		PVZ::Board GetBoard()
		{
			return(PVZ::Board(Memory::ReadMemory<int>(BaseAddress + 4)));
		}
		/// @brief X 坐标。部分派生类仅将其用作实际坐标的取整版本。
		INT_PROPERTY(ImageX, __get_ImageX, __set_ImageX, 8);
		/// @brief Y 坐标。部分派生类仅将其用作实际坐标的取整版本。
		INT_PROPERTY(ImageY, __get_ImageY, __set_ImageY, 0xC);
		/// @brief 宽度
		INT_PROPERTY(Width, __get_Width, __set_Width, 0x10);
		/// @brief 高度
		INT_PROPERTY(Height, __get_Height, __set_Height, 0x14);
		/// @brief 是否可见
		T_PROPERTY(BOOLEAN, Visible, __get_Visible, __set_Visible, 0x18);
		/// @brief 行
		INT_PROPERTY(Row, __get_Row, __set_Row, 0x1C);
		/// @brief 绘制图层编号
		INT_PROPERTY(Layer, __get_Layer, __set_Layer, 0x20);
	};
	class TrackInstance;
	class AttachEffect : public BaseClass
	{
	public:
		AttachEffect(int address) : BaseClass(address) {};
		Matrix3 GetOffset();
	};
	/// @brief 动画部件。相当于其他游戏的模型。
	class Animation : public BaseClass
	{
	public:
		Animation(int idoraddress);
		//support muiti-animprop(AP_XXXXXX)
		static void UnLock(int animprop);
		static void Lock();
		int GetBaseAddress();
		/// @brief 动画类型
		T_READONLY_PROPERTY(AnimationType::AnimationType, Type, __get_Type, 0);
		/// @brief 动画时长
		T_PROPERTY(FLOAT, CycleRate, __get_CycleRate, __set_CycleRate, 4);
		/// @brief 播放速率
		T_PROPERTY(FLOAT, Speed, __get_Speed, __set_Speed, 8);
		/// @brief 是否已移除
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x14);
		/// @brief 起始帧
		INT_PROPERTY(StartFrame, __get_StartFrame, __set_StartFrame, 0x18);
		/// @brief 帧数
		INT_PROPERTY(FrameCount, __get_FrameCount, __set_FrameCount, 0x1C);
		INT_PROPERTY(FrameBasePose, __get_FrameBasePose, __set_FrameBasePose, 0x20);
		T_PROPERTY(FLOAT, XScale, __get_XScale, __set_XScale, 0x24);
		T_PROPERTY(FLOAT, XSlant, __get_XSlant, __set_XSlant, 0x28);
		T_PROPERTY(FLOAT, XOffset, __get_XOffset, __set_XOffset, 0x2C);
		T_PROPERTY(FLOAT, YScale, __get_YScale, __set_YScale, 0x30);
		T_PROPERTY(FLOAT, YSlant, __get_YSlant, __set_YSlant, 0x34);
		T_PROPERTY(FLOAT, YOffset, __get_YOffset, __set_YOffset, 0x38);
		/// @brief 获取该动画的颜色重载。
		/// @return 颜色
		Color GetColor(); // 0x48-0x54，基础颜色
		/// @brief 设置该动画的颜色重载
		/// @param color 设置后的颜色
		void SetColor(Color color);
		/// @brief 获取指定名称的轨道
		/// @param trackName 轨道名称
		/// @return 轨道实例
		TrackInstance GetTrackInstance(const char* trackName);
		/// @brief 循环播放次数
		INT_PROPERTY(CycleCount, __get_CycleCount, __set_CycleCount, 0x5C);
		/// @brief 获取该动画的加色
		/// @return 加色
		Color GetAdditiveColor();
		/// @brief 设置该动画的加色
		/// @param color 设置后动画的加色
		void SetAdditiveColor(Color color);
		/// @brief 是否启用加色模式
		T_PROPERTY(BOOLEAN, DrawAdditiveColor, __get_DrawAdditiveColor, __set_DrawAdditiveColor, 0x7C);
		/// @brief 获取该动画的覆盖色
		/// @return 覆盖色
		Color GetOverlayColor();
		/// @brief 设置该动画的覆盖色
		/// @param color 设置后动画的覆盖色
		void SetOverlayColor(Color color);
		/// @brief 是否启用覆盖颜色模式
		T_PROPERTY(BOOLEAN, DrawOverlayColor, __get_DrawOverlayColor, __set_DrawOverlayColor, 0x90);
		/// @brief 设置染色模式，主要实现模仿者的染色
		T_PROPERTY(PaintState::PaintState, Paint, __get_Paint, __set_Paint, 0x98);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x9C);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 将该模型附加在指定识别 ID 的附件上。
		/// @param attachmentID 附件的识别 ID
		/// @param OffsetX X 坐标偏移
		/// @param OffsetY Y 坐标偏移
		/// @return 附加后的效果，可以用于设置变换矩阵
		AttachEffect AttachTo(AttachmentID attachmentID, float OffsetX, float OffsetY);
		/// @brief 移除该动画部件
		void Die();
		/// @brief 播放指定动画
		/// @param TrackName 动画轨道名称
		/// @param blendType 混合类型
		/// @param loopType 循环类型
		/// @param rate 播放帧频
		void Play(const char* TrackName, int blendType, int loopType, float rate);
		/// @brief 设置指定前缀的所有轨道在绘制时的分组。通常情况下，分组为 -1 时表示隐藏该轨道。
		/// @param trackName 动作轨道的名称前缀
		/// @param renderGroup 分组大小
		void AssignRenderGroupToPrefix(byte RenderGroup, const char* TrackName);
		/// @brief 设置指定动画轨道在绘制时的分组。通常情况下，分组为 -1 时表示隐藏该轨道。
		/// @param trackName 执行的动作轨道名称
		/// @param renderGroup 分组大小
		void AssignRenderGroupToTrack(const char* trackName, byte renderGroup);
		int FindTrackIndex(const char* trackName);

		/// @brief 令动画部件执行 trackName 动作。
		/// @param trackName 执行的动作轨道名称。
		void SetFramesForLayer(const char* theTrackName);
		/// @brief 设置指定轨道的重载图片
		/// @param theTrackName 轨道名称
		/// @param theImage 图片
		void SetImageOverride(const char* theTrackName, Image theImage);
	};
	class Attachment : public BaseClass
	{
	public:
		Attachment(int idoraddress);
		// TODO: check whether this function works properly.
		PVZ::Animation GetAnimation();
		INT_READONLY_PROPERTY(Id, __get_Id, 0x308);
	};
	class AttachmentID : public BaseClass
	{
	public:
		AttachmentID(DWORD address) : BaseClass(address) {};
	};
	class TrackInstance : public BaseClass
	{
	public:
		TrackInstance(int idoraddress);
		AttachmentID GetAttachmentID();
		Attachment GetAttachment();
	};
	/// @brief 控制行地形类型和每个格位地形类型的类
	class Lawn : public BaseClass
	{
	public:
		Lawn(int baseaddress);
		/// @brief 获取指定格位的地形类型
		/// @param row 行
		/// @param column 列
		/// @return 地形类型
		LawnType::LawnType GetGridType(int row, int column);
		/// @brief 设置指定格位的地形类型
		/// @param row 行
		/// @param column 列
		/// @param type 设置后的地形类型
		void SetGridType(int row, int column, LawnType::LawnType type);
		/// @brief 获取指定行的地形类型
		/// @param route 行
		/// @return 地形类型
		RouteType::RouteType GetRouteType(int route);
		/// @brief 设置指定行的地形类型
		/// @note 该函数不会影响该行任何格位的地形类型，需要用 SetGridType() 另行改动。
		/// @param route 行
		/// @param type 设置后的地形类型
		void SetRouteType(int route, RouteType::RouteType type);
		/// @brief 判断指定类型卡牌是否可以在指定位置上使用。
		/// @param row 行
		/// @param column 列
		/// @param type 卡牌类型
		/// @return 是否可以使用
		bool Plantable(int row, int column, SeedType::SeedType type);
	};
	/// @brief 冰道
	class Icetrace : public BaseClass
	{
	public:
		Icetrace(int baseaddress);
		/// @brief 获取指定行的冰道最左侧 X 坐标
		/// @param route 行
		/// @return 冰道最左侧 X 坐标
		int GetX(int route);
		/// @brief 设置指定行的冰道最左侧 X 坐标
		/// @param route 行
		/// @param x 设置后的 X 坐标
		void SetX(int route, int x);
		/// @brief 获取指定行的冰道消失倒计时
		/// @param route 行
		/// @return 消失倒计时
		int GetDisappearCountdown(int route);
		/// @brief 设置指定行的冰道消失倒计时
		/// @param route 行
		/// @param cs 设置的消失倒计时，单位为厘秒。
		void SetDisappearCountdown(int route, int cs);
	};
	/// @brief 一波僵尸的出怪列表
	class Wave : public BaseClass
	{
	public:
		Wave(int baseaddress);
		/// @brief 此波的总僵尸数
		READONLY_PROPERTY(int, __get_Count) Count;
		/// @brief 获取此波所有僵尸
		/// @param ztypes 存储返回值的数组
		void GetAll(ZombieType::ZombieType* ztypes);
		/// @brief 设置此波僵尸。若不足 50 个，应当用 ZombieType::None 结尾。
		/// @param ztypes 僵尸列表
		/// @param length 僵尸列表长度
		void SetAll(ZombieType::ZombieType* ztypes, size_t length);
		/// @brief 获取出怪列表指定编号的僵尸
		/// @param index 僵尸在出怪列表中的编号
		/// @return 僵尸类型
		ZombieType::ZombieType Get(int index);
		/// @brief 设置指定编号上的僵尸。
		/// @note 不能以此法增加僵尸数量。若要增加，请使用 Add()
		/// @param index 
		/// @param ztype 
		void Set(int index, ZombieType::ZombieType ztype);
		/// @brief 删除指定编号上的僵尸
		/// @param index 编号
		void Del(int index);
		/// @brief 将指定类型的僵尸添加到列表末尾。
		/// @param ztype 僵尸类型
		void Add(ZombieType::ZombieType ztype);
		/// @brief 将指定数组的全部僵尸加到本波出怪列表中。溢出的部分会被忽略。
		/// @param ztypes 僵尸类型数组
		/// @param length 数组长度
		void AddAll(ZombieType::ZombieType* ztypes, int length);
	};
	// 鼠标对象(控制层面的鼠标)
	class Mouse : public BaseClass
	{
	public:
		Mouse(int baseaddress) : BaseClass(baseaddress) {};
		T_READONLY_PROPERTY(BOOLEAN, InGameArea, __get_InGameArea, 0xDC);
		INT_PROPERTY(X, __get_X, __set_X, 0xE0);
		INT_PROPERTY(Y, __get_Y, __set_Y, 0xE4);
		T_READONLY_PROPERTY(MouseClickState::MouseClickState, ClickState, __get_ClickState, 0xE8);
		void WMClick(short x, short y);
		void GameClick(int x, int y);
		void MoveTo(int x, int y);
	};
	/// @brief 僵尸
	class Zombie : public GameObject
	{
	public:
		Zombie(int indexoraddress);
		/// @brief 植物的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static const DWORD MemSize = 0x15C;
		/// @brief 调整该类在 PVZ 中对象的大小。
		/// @note 请在派生类中调用这个函数。
		/// @note 调用该函数后，新生成的存档与原版存档不兼容，请注意清理。
		/// @param MemSize 更改后的大小。
		/// @param NewCount 调整后僵尸上限数
		static void SetMemSize(int NewSize, int NewCount);

		/// @deprecated
		struct AccessoriesType1
		{
			HelmType::HelmType Type;
			int Hp;
			int MaxHp;
		};
		/// @deprecated
		struct AccessoriesType2
		{
			ShieldType::ShieldType Type;
			int Hp;
			int MaxHp;
		};

		/// @brief 僵尸类型
		T_PROPERTY(ZombieType::ZombieType, Type, __get_Type, __set_Type, 0x24);
		/// @brief 僵尸状态
		T_PROPERTY(ZombieState::ZombieState, State, __get_State, __set_State, 0x28);
		/// @brief 实际 X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 0x2C);
		/// @brief 实际 Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0x30);
		/// @brief 僵尸移动速度
		PROPERTY(FLOAT, __get_Speed, SetSpeed) Speed;
		/// @brief 是否显示舌头
		T_READONLY_PROPERTY(BOOLEAN, ShowingTongue, __get_ShowingTongue, 0x50);
		/// @brief 是否在吃东西
		T_READONLY_PROPERTY(BOOLEAN, Eating, __get_Eating, 0x51);
		void Light(int cs = 100);
		INT_PROPERTY(JustGotShotCounter, __get_JustGotShotCounter, __set_JustGotShotCounter, 0x54);
		INT_PROPERTY(ShieldJustGotShotCounter, __get_ShieldJustGotShotCounter, __set_ShieldJustGotShotCounter, 0x58);
		/// @brief 存在时间
		INT_READONLY_PROPERTY(ExistedTime, __get_ExistedTime, 0x60);
		INT_PROPERTY(ZombieHeight, __get_ZombieHeight, __set_ZombieHeight, 0x64);
		/// @brief 属性倒计时
		INT_PROPERTY(AttributeCountdown, __get_AttributeCountdown, __set_AttributeCountdown, 0x68);
		/// @brief 是否生成过掉落物
		T_PROPERTY(BOOLEAN, DroppedLoot, __get_DroppedLoot, __set_DroppedLoot, 0x70);
		/// @brief 消失倒计时
		INT_PROPERTY(DisappearCountdown, __get_DisappearCountdown, __set_DisappearCountdown, 0x74);
		/// @brief 蹦极僵尸目标列
		INT_PROPERTY(BungeeColumn, __get_BungeeColumn, __set_BungeeColumn, 0x80);
		/// @brief 高度
		T_PROPERTY(FLOAT, Height, __get_Height, __set_Height, 0x84);
		void GetCollision(CollisionBox* collbox);
		void SetCollision(CollisionBox* collbox);
		/// @brief 获取僵尸的基础攻击判定范围。
		/// @param collbox 攻击判定范围的存放位置。其中 X 和 Y 为相应坐标的偏移量。
		void GetAttackCollision(CollisionBox* collbox);
		/// @brief 设置僵尸的基础攻击判定范围。
		/// @param collbox 攻击判定范围的指针。其中 X 和 Y 为相应坐标的偏移量。
		void SetAttackCollision(CollisionBox* collbox);
		/// @brief 冰减速持续时间
		INT_PROPERTY(DecelerateCountdown, __get_DecelerateCountdown, __set_DecelerateCountdown, 0xAC);
		/// @brief 黄油定身持续时间
		INT_PROPERTY(FixedCountdown, __get_FixedCountdown, __set_FixedCountdown, 0xB0);
		/// @brief 冻结持续时间
		INT_PROPERTY(FrozenCountdown, __get_FrozenCountdown, __set_FrozenCountdown, 0xB4);
		/// @brief 是否被魅惑
		T_PROPERTY(BOOLEAN, Hypnotized, __get_Hypnotized, __set_Hypnotized, 0xB8);
		/// @brief 是否被吹飞
		T_PROPERTY(BOOLEAN, Blowaway, __get_Blowaway, __set_Blowaway, 0xB9);
		/// @brief 是否有头
		T_PROPERTY(BOOLEAN, NotDying, __get_NotDying, __set_NotDying, 0xBA);
		/// @brief 是否有手臂
		T_READONLY_PROPERTY(BOOLEAN, HandBroken, __get_HandBroken, 0xBB);
		/// @brief 是否有特殊物件
		T_PROPERTY(BOOLEAN, SthinHandOrYetiLeft, __get_SthinHandOrYetiLeft, __set_SthinHandOrYetiLeft, 0xBC);
		/// @brief 是否在水中
		T_PROPERTY(BOOLEAN, InWater, __get_InWater, __set_InWater, 0xBD);
		/// @brief 是否被大蒜影响
		T_PROPERTY(BOOLEAN, GarlicBited, __get_GarlicBited, __set_GarlicBited, 0xBF);
		/// @brief 头盔防具类型
		T_PROPERTY(HelmType::HelmType, HelmType, __get_HelmType, __set_HelmType,		0x0C4);
		/// @brief 本体生命值
		INT_PROPERTY(BodyHealth,		__get_BodyHealth,		__set_BodyHealth,		0x0C8);
		/// @brief 本体生命上限
		INT_PROPERTY(BodyMaxHealth,		__get_BodyMaxHealth,	__set_BodyMaxHealth,	0x0CC);
		/// @brief 头盔生命值
		INT_PROPERTY(HelmHealth,		__get_HelmHealth,		__set_HelmHealth,		0x0D0);
		/// @brief 头盔生命上限
		INT_PROPERTY(HelmMaxHealth,		__get_HelmMaxHealth,	__set_HelmMaxHealth,	0x0D4);
		/// @brief 盾牌防具类型
		T_PROPERTY(ShieldType::ShieldType, ShieldType, __get_ShieldType, __set_ShieldType, 0x0D8);
		/// @brief 盾牌生命值
		INT_PROPERTY(ShieldHealth,		__get_ShieldHealth,		__set_ShieldHealth,		0x0DC);
		/// @brief 盾牌生命上限
		INT_PROPERTY(ShieldMaxHealth,	__get_ShieldMaxHealth,	__set_ShieldMaxHealth,	0x0E0);
		/// @brief 气球生命值
		INT_PROPERTY(FlyingHealth, __get_FlyingHealth, __set_FlyingHealth, 0x0E4);
		/// @brief 气球生命上限
		INT_PROPERTY(FlyingMaxHealth, __get_FlyingMaxHealth, __set_FlyingMaxHealth, 0xE8);
		/// @brief 是否已移除
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0xEC);
		/// @brief 关联僵尸的识别 ID
		INT_PROPERTY(RelatedZombieID, __get_RelatedZombieID, __set_RelatedZombieID, 0x0F0);
		/// @brief 获取僵尸动画
		/// @return 僵尸动画
		PVZ::Animation GetAnimation();
		/// @brief 僵王召唤僵尸的倒计时。同时也是投手僵尸剩余篮球数。
		INT_PROPERTY(SummonCounter, __get_SummonCounter, __set_SummonCounter, 0x114);
		/// @brief 大小
		T_PROPERTY(FLOAT, Size, __get_Size, __set_Size, 0x11C);
		//临时变量
		INT_PROPERTY(Temp, __get_Temp, __set_Temp, 0x12C);
		/// @brief 获取特殊头部动画（如植物僵尸的植物头）
		/// @return 特殊头部动画
		Animation GetSpecialHeadAnimation();
		/// @brief 设置特殊头部动画（如植物僵尸的植物头）
		/// @param anim 特殊头部动画
		void SetSpecialHeadAnimation(Animation anim);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x158);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 对僵尸造成伤害
		/// @param damage 伤害
		/// @param flags 伤害标签
		void Hit(int damage, DamageFlags flags = DAMAGEF_NONE);
		/// @brief 忽略所有护甲，对僵尸本体造成伤害
		/// @param damage 伤害
		/// @param flags 伤害标签
		void HitBody(int damage, DamageFlags flags = DAMAGEF_NONE);
		/// @brief 令僵尸化灰，对本体生命值大于 1800 的僵尸造成 1800 点伤害。
		void Blast();
		/// @brief 对僵尸施加黄油效果
		/// @param countdown 持续时间
		void Butter(int countdown);
		/// @brief 减速僵尸，无法减速默认免疫减速的僵尸。
		/// @param countdown 持续时间
		void Decelerate(int countdown);
		/// @brief 冻结僵尸，无法冻结默认免疫冻结的僵尸。
		/// @note 不造成冻结伤害。\n
		///		不影响减速时长。
		/// @param countdown 持续时间
		void Froze(int countdown);
		/// @brief 魅惑此僵尸
		void Hypnotize();
		/// @brief 移除僵尸
		void Remove();
		/// @brief 移除僵尸并生成它的掉落物。
		void RemoveWithLoot();
		//LoopType(APA_XXXXXX)
		void SetAnimation(LPCSTR animName, byte LoopType, int blend_time = 14, float fps = 24.0f);
		/// @brief 为僵尸装备铁桶
		/// @param shield 铁桶生命值
		void EquipBucket(int shield = 1100);
		/// @brief 为僵尸装备路障
		/// @param shield 路障生命值
		void EquipCone(int shield = 370);
		/// @brief 设置僵尸动画轨道在绘制时的渲染分组。
		/// @param trackPrefix 受影响轨道的名称的前缀。
		/// @param renderGroup 调整后轨道组号。组号越大则图层越上层，为 -1 时隐藏。
		void ReanimShowPrefix(const char* trackPrefix, int renderGroup);
		/// @brief 僵尸是否可被冰减速
		/// @return 是否可被冰减速
		bool canDecelerate();
		/// @brief 僵尸是否可被冻结
		/// @todo 将该函数改为完全调用内置函数的版本。
		/// @return 是否可被冻结
		bool canFroze();
		/// @brief 判定僵尸是否能被指定索敌类型搜寻到。
		/// @param range 索敌类型。
		/// @param usepvzfunc 是否调用 pvz 内部函数。默认为 true。
		/// @return 是否能被搜寻到。
		bool EffectedBy(DamageRangeFlags range, bool usepvzfunc = true);
		/// @brief 僵尸运动或状态改变时调用，更新动画速度。
		void UpdateAnimSpeed();

		/// @brief 获取僵尸的实际可攻击范围。
		/// @return 僵尸的实际攻击范围
		Rect GetActualAttackRect();
		/// @brief 获取僵尸的实际受击范围。
		/// @return 僵尸的实际受击范围
		Rect GetActualRect();

		/// @brief 设置是否显示铁门僵尸的手臂。
		/// @param shown 是否显示，默认为 true
		void ShowDoorArms(bool shown = true);
		/// @brief 根据盾的类型设置相应动画轨道的绘制分组。
		void AttachShield();

		/// @deprecated
		void GetBodyHp(int* hp, int* maxhp);
		/// @deprecated
		void SetBodyHp(int hp, int maxhp);
		/// @deprecated
		AccessoriesType1 GetAccessoriesType1();
		/// @deprecated
		void SetAccessoriesType1(AccessoriesType1 acctype1);
		/// @deprecated
		AccessoriesType2 GetAccessoriesType2();
		/// @deprecated
		void SetAccessoriesType2(AccessoriesType2 acctype2);
	};
	/// @brief 子弹
	class Projectile : public GameObject
	{
	public:
		/// @brief 子弹的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static const DWORD MemSize = 0x94;
		Projectile(int indexoraddress);
		/// @brief 实际 X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 0x30);
		/// @brief 子弹本体的 Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0x34);
		/// @brief 子弹 Z 坐标
		T_PROPERTY(FLOAT, Height, __get_Height, __set_Height, 0x38);
		/// @brief X 方向坐标
		T_PROPERTY(FLOAT, XSpeed, __get_XSpeed, __set_XSpeed, 0x3C);
		T_PROPERTY(FLOAT, YSpeed, __get_YSpeed, __set_YSpeed, 0x40);
		T_PROPERTY(FLOAT, HeightSpeed, __get_HeightSpeed, __set_HeightSpeed, 0x44);
		T_PROPERTY(FLOAT, HeightAcceleration, __get_HeightAcceleration, __set_HeightAcceleration, 0x48);
		/// @brief 影子 Y 坐标
		T_PROPERTY(FLOAT, ShadowY, __get_ShadowY, __ShadowY, 0x4C);
		/// @brief 是否已移除
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x50);
		/// @brief 子弹运动类型
		T_PROPERTY(MotionType::MotionType, Motion, __get_Motion, __set_Motion, 0x58);
		/// @brief 子弹类型
		T_PROPERTY(ProjectileType::ProjectileType, Type, __get_Type, __set_Type, 0x5C);
		/// @brief 存在时间
		INT_READONLY_PROPERTY(ExistedTime, __get_ExistedTime, 0x60);
		/// @brief 旋转大小，暂不确定单位是否为弧度。
		T_PROPERTY(FLOAT, RotationAngle, __get_RotationAngle, __set_RotationAngle, 0x68);
		/// @brief 旋转速度，尚不确认是否是角速度。
		T_PROPERTY(FLOAT, RotationSpeed, __get_RotationSpeed, __set_RotationSpeed, 0x6C);
		/// @brief 子弹索敌标签
		T_PROPERTY(DamageRangeFlags, DamageAbility, __get_DamageAbility, __set_DamageAbility, 0x74);
		/// @brief 对于跟踪弹道，表示跟踪目标僵尸的 ID 。
		INT_PROPERTY(TracktargetId, __get_TracktargetId, __set_TracktargetId, 0x88);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x90);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 子弹过火，转化为火球
		void OnFire();
		/// @brief 移除该子弹。
		void Remove();
	};
	/// @brief 植物
	class Plant : public GameObject
	{
	public:
		/// @brief 植物的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static const int MemSize = 0x14C;
		Plant(int indexoraddress);
		/// @brief 调整该类在 PVZ 中对象的大小。
		/// @note 请在派生类中调用这个函数。
		/// @note 调用该函数后，新生成的存档与原版存档不兼容，请注意清理。
		/// @param MemSize 更改后的大小。
		/// @param NewCount 调整后植物上限数
		static void SetMemSize(int NewSize, int NewCount);
		/// @brief 类型
		T_PROPERTY(SeedType::SeedType, Type, __get_Type, __set_Type, 0x24);
		/// @brief 列
		INT_PROPERTY(Column, __get_Column, __set_Column, 0x28);
		/// @brief 植物状态
		/// @see PlantState
		T_PROPERTY(PlantState::PlantState, State, __get_State, __set_State, 0x3C);
		/// @brief 当前生命值
		INT_PROPERTY(Hp, __get_Hp, __set_Hp, 0x40);
		/// @brief 最大生命值
		INT_PROPERTY(MaxHp, __get_MaxHp, __set_MaxHp, 0x44);
		/// @brief 植物子类别
		INT_PROPERTY(SubClass, __get_SubClass, __set_SubClass, 0x48);
		/// @deprecated
		T_PROPERTY(BOOLEAN, Aggressive, __get_Aggressive, __set_Aggressive, 0x48);
		/// @brief 消失倒计时
		INT_PROPERTY(BloverDisappearCountdown, __get_BloverDisappearCountdown, __set_BloverDisappearCountdown, 0x4C);
		/// @brief 一次性植物发动技能的倒计时
		INT_PROPERTY(EffectiveCountdown, __get_EffectiveCountdown, __set_EffectiveCountdown, 0x50);
		/// @brief 各种倒计时
		INT_PROPERTY(AttributeCountdown, __get_AttributeCountdown, __set_AttributeCountdown, 0x54);
		/// @brief 植物射击（或产出物品）的倒计时
		INT_PROPERTY(ShootOrProductCountdown, __get_ShootOrProductCountdown, __set_ShootOrProductCountdown, 0x58);
		/// @brief 植物射击（或产出物品）的基础间隔
		INT_PROPERTY(ShootOrProductInterval, __get_ShootOrProductInterval, __set_ShootOrProductInterval, 0x5C);
		/// @brief 目标 X 坐标
		INT_PROPERTY(mTargetX, __get_mTargetX, __set_mTargetX, 0x80);
		/// @brief 目标 Y 坐标
		INT_PROPERTY(mTargetY, __get_mTargetY, __set_mTargetY, 0x84);
		/// @brief 粒子效果识别 ID
		T_PROPERTY(DWORD, ParticleID, __get_ParticleID, __set_ParticleID, 0x8C);
		/// @brief 射击动作倒计时
		INT_PROPERTY(ShootingCountdown, __get_ShootingCountdown, __set_ShootingCountdown, 0x90);
		/// @brief 获取植物的第一个动画
		/// @return 第一个动画
		PVZ::Animation GetAnimationPart1();
		PVZ::Animation GetAnimationPart2();
		PVZ::Animation GetAnimationPart3();
		PVZ::Animation GetAnimationPart4();
		/// @brief 获取植物的眨眼动画
		/// @return 植物的眨眼动画
		PVZ::Animation GetAnimationEyeBlink();
		PVZ::Animation GetAnimationPotatoGlow();
		/// @brief 获取植物的睡眠动画
		/// @return 植物的睡眠动画
		PVZ::Animation GetAnimationSleep();
		void Light(int cs = 100);
		void Flash(int cs = 100);
		T_PROPERTY(FLOAT, ImageXOffset, __get_ImageXOffset, __set_ImageXOffset, 0xC0);
		T_PROPERTY(FLOAT, ImageYOffset, __get_ImageYOffset, __set_ImageYOffset, 0xC4);
		/// @brief 目标僵尸的识别 ID
		T_PROPERTY(DWORD, mTargetZombieID, __get_mTargetZombieID, __set_mTargetZombieID, 0x12C);
		/// @brief 苏醒倒计时
		INT_PROPERTY(mWakeUpCounter, __get_mWakeUpCounter, __set_mWakeUpCounter, 0x130);
		/// @brief 被蹦极抱起的状态
		INT_PROPERTY(mOnBungee, __get_mOnBungee, __set_mOnBungee, 0x134);
		/// @brief 是否已消失
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x141);
		/// @brief 是否已被压扁
		T_PROPERTY(BOOLEAN, Squash, __get_Squash, __set_Squash, 0x142);
		/// @brief 是否已睡着
		T_READONLY_PROPERTY(BOOLEAN, Sleeping, __get_Sleeping, 0x143);
		/// @brief 设置植物是否睡着
		/// @param sleeping 植物是否睡着
		void SetSleeping(bool sleeping);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x148);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		void CreateEffect();
		/// @brief 将植物定身为纸板（同 IZ）。
		/// @note 会让土豆地雷直接出土。
		void SetStatic();
		/// @brief 压扁该植物
		void Smash();
		/// @brief 根据植物的当前位置，计算植物应当处于的图层编号
		/// @return 植物应当所在的图层编号
		int CalcLayer();
		/// @brief 移动至指定位置，并更新相关属性
		/// @param row 行
		/// @param column 列
		void MoveTo(int row, int column);
		/// @brief 移除该植物
		void Remove();
		/// @deprecated
		PVZ::Projectile Shoot(int targetid = -1);
		/// @brief 立刻发射子弹
		/// @param motiontype 子弹移动类型
		/// @param targetid 攻击目标僵尸的 ID
		/// @param special 是否使用副武器进行攻击
		/// @return 生成的子弹
		PVZ::Projectile Shoot(MotionType::MotionType motiontype = MotionType::None, int targetid = -1, bool special = false);
		//animPlayArg(APA_XXXXXX)
		void SetAnimation(LPCSTR animName, byte animPlayArg, int imagespeed);
		/// @brief 以指定帧频播放闲置动画。IZ 关卡中动画速率会设为 0 。
		/// @param speed 指定的帧频
		void PlayIdleAnim(float speed);
		class MagnetItem
		{
			int BaseAddress;
		public:
			MagnetItem(int address);
			T_PROPERTY(FLOAT, X, __get_X, __set_X, 0);
			T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 4);
			T_PROPERTY(FLOAT, DestOffsetX, __get_DestOffsetX, __set_DestOffsetX, 8);
			T_PROPERTY(FLOAT, DestOffsetY, __get_DestOffsetY, __set_DestOffsetY, 0xC);
			T_PROPERTY(MagnetItemType::MagnetItemType, Type, __get_Type, __set_Type, 0x10);
		};
		MagnetItem GetMagnetItem(int num);

		/// @brief 取得植物种植时的基础阳光消耗。
		/// @note 对其他类型的卡牌也有效
		/// @param type 种子卡类型
		/// @param imitater_type 模仿者模仿的类型
		/// @return 种植的基础阳光消耗
		static int GetCost(SeedType::SeedType type, SeedType::SeedType imitater_type = SeedType::None);
	};
	class GardenPlant : public BaseClass
	{
	public:
		GardenPlant(int address);
		T_PROPERTY(SeedType::SeedType, Type, __get_Type, __set_Type, 8);
		T_PROPERTY(GardenScene::GardenScene, Location, __get_Location, __set_Location, 0xC);
		INT_PROPERTY(Column, __get_Column, __set_Column, 0x10);
		INT_PROPERTY(Row, __get_Row, __set_Row, 0x14);
		//GPD_LEFT or GPD_RIGTHT
		INT_PROPERTY(Direction, __get_Direction, __set_Direction, 0x18);
		T_PROPERTY(std::time_t, LastWateredTime, __get_LastWateredTime, __set_LastWateredTime, 0x20);
		INT_PROPERTY(Colour, __get_Colour, __set_Colour, 0x28);
		T_PROPERTY(GardenPlantState::GardenPlantState, State, __get_State, __set_State, 0x2C);
		INT_PROPERTY(WateredCount, __get_WateredCount, __set_WateredCount, 0x30);
		INT_PROPERTY(LevelUpWaterNeed, __get_LevelUpWaterNeed, __set_LevelUpWaterNeed, 0x34);
		T_PROPERTY(GardenPlantNeed::GardenPlantNeed, LevelUpNeed, __get_LevelUpNeed, __set_LevelUpNeed, 0x38);
		T_PROPERTY(std::time_t, LastGoldTime, __get_LastGoldTime, __set_LastGoldTime, 0x40);
		T_PROPERTY(std::time_t, LastGrowthTime, __get_LastGrowthTime, __set_LastGrowthTime, 0x48);
	};
	/// @brief 各种掉落物
	class Coin : public GameObject
	{
	public:
		/// @brief 掉落物的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static const DWORD MemSize = 0x0D8;
		Coin(int indexoraddress);
		INT_READONLY_PROPERTY(ImageXVariation, __get_ImageXVariation, 8);
		INT_READONLY_PROPERTY(ImageYVariation, __get_ImageYVariation, 0xC);
		void GetCollision(CollisionBox* collbox);
		void SetCollision(CollisionBox* collbox);
		/// @brief 实际 X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 0x24);
		/// @brief 实际 Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0x28);
		/// @brief 大小
		T_PROPERTY(FLOAT, Size, __get_Size, __set_Size, 0x34);
		/// @brief 是否已消失
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x38);
		/// @brief 已存在时间
		INT_READONLY_PROPERTY(ExistedTime, __get_ExistedTime, 0x4C);
		/// @brief 是否已被收集
		T_PROPERTY(BOOLEAN, Collected, __get_Collected, __set_Collected, 0x50);
		/// @brief 消失倒计时
		INT_PROPERTY(DisappearCounter, __get_DisappearCounter, __set_DisappearCounter, 0x54);
		/// @brief 物品类型
		T_PROPERTY(CoinType::CoinType, Type, __get_Type, __set_Type, 0x58);
		/// @brief 该物品的移动类型
		T_PROPERTY(CoinMotionType::CoinMotionType, Motion, __get_Motion, __set_Motion, 0x5C);
		/// @brief 获取该物品的附件
		/// @return 该物品的附件
		PVZ::Attachment GetAttachment();
		/// @brief 种子卡片的种子类型
		T_PROPERTY(SeedType::SeedType, ContentCard, __get_ContentCard, __set_ContentCard, 0x68);
		/// @brief 获取该掉落物对应的花园盆栽
		/// @return 该掉落物对应的花园盆栽
		PVZ::GardenPlant GetGardenPlant();
		/// @brief 是否有光环视觉效果
		T_PROPERTY(BOOLEAN, HasHalo, __get_HasHalo, __set_HasHalo, 0xC8);
		/// @brief 识别 ID.
		INT_READONLY_PROPERTY(Id, __get_Id, 0xD4);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		void Collect();
		void Die();
	};
	/// @brief 除草机
	class LawnMower : public BaseClass
	{
	public:
		/// @brief 除草机的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static const DWORD MemSize = 0x48;
		LawnMower(int indexoraddress);
		/// @brief X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 8);
		/// @brief Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0xC);
		/// @brief 绘制图层编号
		INT_PROPERTY(Layer, __get_Layer, __set_Layer, 0x10);
		/// @brief 行
		INT_PROPERTY(Row, __get_Row, __set_Row, 0x14);
		/// @brief 获取除草机的动画
		/// @return 除草机的动画
		PVZ::Animation GetAnimation();
		/// @brief 除草机状态
		T_PROPERTY(LawnmoverState::LawnmoverState, State, __get_State, __set_State, 0x2C);
		/// @brief 是否已消失
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x30);
		/// @brief 是否可见
		T_PROPERTY(BOOLEAN, Visible, __get_Visible, __set_Visible, 0x31);
		/// @brief 除草机类型
		T_PROPERTY(LawnmoverType::LawnmoverType, Type, __get_Type, __set_Type, 0x34);
		/// @brief Y 坐标偏移量
		T_PROPERTY(FLOAT, YOffset, __get_YOffset, __set_YOffset, 0x38);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0x44);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 移除该除草机
		void Die();
	};
	using Lawnmover = LawnMower;
	/// @brief 各种场地物件的基类。
	class Griditem : public BaseClass
	{
	public:
		/// @brief 场地物件的内存占用字节数。\n
		///		若派生类需要对应扩指针的对象，请在派生类中修改此数值。
		static const DWORD MemSize = 0x0EC;
		/// @brief 默认的场地物件类型，派生类需要定义同名常量，用于在 Board::GetAllGriditems() 中定向获取场地物件。
		static const GriditemType::GriditemType ItemType = GriditemType::None;
		Griditem(int indexoraddress);
		/// @brief 获取场地物件所在的 Board
		/// @return 场地物件所在的 Board
		PVZ::Board GetBoard();
		/// @brief 场地物件类型
		T_PROPERTY(GriditemType::GriditemType, Type, __get_Type, __set_Type, 0x8);
		/// @brief 场地物件状态
		T_PROPERTY(GriditemState::GriditemState, State, __get_State, __set_State, 0xC);
		/// @brief 列
		INT_PROPERTY(Column, __get_Column, __set_Column, 0x10);
		/// @brief 行
		INT_PROPERTY(Row, __get_Row, __set_Row, 0x14);
		/// @brief 绘制图层编号
		INT_PROPERTY(Layer, __get_Layer, __set_Layer, 0x1C);
		/// @brief 是否已消失
		/// @attention 你应该通过 Remove() 移除一个场地物件，而不是通过修改此变量。
		T_PROPERTY(BOOLEAN, NotExist, __get_NotExist, __set_NotExist, 0x20);
		/// @brief 识别 ID
		INT_READONLY_PROPERTY(Id, __get_Id, 0xE8);
		READONLY_PROPERTY_BINDING(int, __get_Index, Id & 0xFFFF) Index;
		/// @brief 移除此物件
		void Remove();
	};
	/// @brief 墓碑
	class Grave :public PVZ::Griditem
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::Grave;
		Grave(int indexoraddress) :Griditem(indexoraddress) {};
		INT_PROPERTY(AppearedValue, __get_AppearedValue, __set_AppearedValue, 0x18);
	};
	/// @brief 弹坑
	class Crater :public PVZ::Griditem
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::Crater;
		Crater(int indexoraddress) :Griditem(indexoraddress) {};
		/// @brief 消失倒计时
		INT_PROPERTY(DisappearCountdown, __get_DisappearCountdown, __set_DisappearCountdown, 0x18);
	};
	/// @brief 水族馆模式的脑子
	class AquariumBrain :public PVZ::Griditem
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::AquariumBrain;
		AquariumBrain(int indexoraddress) :Griditem(indexoraddress) {};
		/// @brief 实际的 X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 0x24);
		/// @brief 实际的 Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0x28);
	};
	/// @brief 禅境花园的蜗牛
	class Snail :public PVZ::Griditem
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::Snail;
		Snail(int indexoraddress) :Griditem(indexoraddress) {};
		/// @brief 实际的 X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 0x24);
		/// @brief 实际的 Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0x28);
		/// @brief 目标 X 坐标
		T_PROPERTY(FLOAT, TargetX, __get_TargetX, __set_TargetX, 0x2C);
		/// @brief 目标 Y 坐标
		T_PROPERTY(FLOAT, TargetY, __get_TargetY, __set_TargetY, 0x30);
	};
	/// @brief 罐子
	class Vase :public PVZ::Griditem
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::Vase;
		Vase(int indexoraddress) :Griditem(indexoraddress) {};
		/// @brief 罐子外观。本质是场地物件的状态。
		T_PROPERTY(VaseSkin::VaseSkin, Skin, __get_Skin, __set_Skin, 0xC);
		/// @brief 内含的僵尸类型
		T_PROPERTY(ZombieType::ZombieType, ContentZombie, __get_ContentZombie, __set_ContentZombie, 0x3C);
		/// @brief 内含的种子卡片类型
		T_PROPERTY(SeedType::SeedType, ContentPlant, __get_ContentPlant, __set_ContentPlant, 0x40);
		/// @brief 罐子实际含有的物品类型
		T_PROPERTY(VaseContent::VaseContent, Content, __get_Content, __set_Content, 0x44);
		/// @brief 是否高亮
		T_READONLY_PROPERTY(BOOLEAN, MouseEnter, __get_MouseEnter, 0x48);
		/// @brief 透明状态倒计时
		INT_PROPERTY(TransparentCountDown, __get_TransparentCountDown, __set_TransparentCountDown, 0x4C);
		/// @brief 储存阳光数
		INT_PROPERTY(Sun, __get_Sun, __set_Sun, 0x50);
		/// @brief 直接开启此罐子
		void Open();
	};
	/// @brief IZ 模式的脑子
	class IZBrain :public PVZ::Griditem
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::IZBrain;
		IZBrain(int indexoraddress) :Griditem(indexoraddress) {};
		/// @brief 脑子剩余生命值
		INT_PROPERTY(Hp, __get_Hp, __set_Hp, 0x18);
		/// @brief X 坐标
		T_PROPERTY(FLOAT, X, __get_X, __set_X, 0x24);
		/// @brief Y 坐标
		T_PROPERTY(FLOAT, Y, __get_Y, __set_Y, 0x28);
	};
	/// @brief 传送门
	/// @note 非传送门关卡，传送门只有画面效果，不会触发传送，\n
	///		因此此类只提供几个方法用于模拟传送门关卡中的传送效果
	class Portal :public PVZ::Griditem
	{
	public:
		Portal(int indexoraddress) :Griditem(indexoraddress) {};
		/// @brief 关闭此传送门
		void Close();
		/// @brief 判定僵尸是否进入这个传送门
		/// @param zombie 被判定的僵尸
		/// @return 僵尸是否进入传送门
		/// @todo 实现一个参数为 PVZ::Zombie 的版本
		bool isZombieIn(std::shared_ptr<PVZ::Zombie> zombie);
		/// @brief 获取僵尸从这个传送门出来时的X坐标
		/// @return 僵尸从这个传送门出来时的X坐标
		int getZombieOutX();
		/// @brief 判定子弹是否进入这个传送门
		/// @param projectile 被判定的子弹
		/// @return 子弹是否进入这个传送门
		bool isProjectileIn(std::shared_ptr<PVZ::Projectile> projectile);
		/// @brief 获取子弹从这个传送门射出时的X坐标
		/// @param 子弹从这个传送门射出时的X坐标
		int getProjectileOutX();
	};
	/// @brief 椭圆形传送门，颜色为蓝框冒白光
	class CirclePortal : public Portal
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::PortalBlue;
		CirclePortal(int indexoraddress) : Portal(indexoraddress) {};
	};
	/// @brief 矩形传送门，颜色为黄框
	class SquarePortal : public Portal
	{
	public:
		static const GriditemType::GriditemType ItemType = GriditemType::PortalYellow;
		SquarePortal(int indexoraddress) : Portal(indexoraddress) {};
	};
	class MousePointer : public GameObject//+138
	{
	public:
		MousePointer(int address);
		INT_PROPERTY(CardIndex, __get_CardIndex, __set_CardIndex, 0x24);
		T_PROPERTY(SeedType::SeedType, ContentCard, __get_ContentCard, __set_ContentCard, 0x28);
		T_PROPERTY(SeedType::SeedType, ContentCardImitative, __get_ContentCardImitative, __set_ContentCardImitative, 0x2C);
		T_PROPERTY(MouseType::MouseType, Type, __get_Type, __set_Type, 0x30);
		INT_PROPERTY(CoinID, __get_CoinID, __set_CoinID, 0x34);
		INT_PROPERTY(GlovePlantID, __get_GlovePlantID, __set_GlovePlantID, 0x38);
		INT_PROPERTY(ImitativePlantID, __get_ImitativePlantID, __set_ImitativePlantID, 0x3C);
		INT_PROPERTY(CobCannonID, __get_CobCannonID, __set_CobCannonID, 0x40);
		INT_PROPERTY(HammerDownCount, __get_HammerDownCount, __set_HammerDownCount, 0x44);
		PVZ::Animation GetAnimation();
		READONLY_PROPERTY_BINDING(int, __get_Row, Memory::ReadPointer(PVZBASEADDRESS + 0x13C, 0x28)) Row;
		READONLY_PROPERTY_BINDING(int, __get_Column, Memory::ReadPointer(PVZBASEADDRESS + 0x13C, 0x24)) Column;
	};
	class Caption : public BaseClass
	{
	public:
		Caption(int address);
		int GetBaseAddress();
		void GetText(char str[]);//str[0x80]
		void SetText(const char str[]);//str[0x80]
		INT_PROPERTY(DisappearCountdown, __get_DisappearCountdown, __set_DisappearCountdown, 0x88);
		T_PROPERTY(CaptionStyle::CaptionStyle, Style, __get_CaptionStyle, __set_CaptionStyle, 0x8C);
	};
	using Advice = Caption;
	/// @brief 卡槽对象
	class CardSlot : public GameObject
	{
	public:
		CardSlot(int address);
		/// @brief 卡槽左上角 X 坐标
		INT_PROPERTY(X, __get_X, __set_X, 8);
		/// @brief 卡槽左上角 Y 坐标
		INT_PROPERTY(Y, __get_Y, __set_Y, 0xC);
		INT_PROPERTY(CollisionLength, __get_CollisionLength, __set_CollisionLength, 0x10);
		/// @brief 卡片数量
		PROPERTY(int, __get_CardsCount, SetCardsCount) CardsCount;
		class SeedCard : public GameObject
		{
		public:
			SeedCard(int address);
			INT_PROPERTY(X, __get_X, __set_X, 0x8);
			INT_PROPERTY(Y, __get_Y, __set_Y, 0xC);
			/// @brief 获取卡牌判定范围
			/// @param collbox 存储返回值的指针
			void GetCollision(CollisionBox* collbox);
			/// @brief 设置卡牌判定范围
			/// @param collbox 设置的判定范围，不影响 X Y 坐标
			void SetCollision(CollisionBox* collbox);
			/// @brief 已冷却时间，从0开始
			INT_PROPERTY(CoolDown, __get_CoolDown, __set_CoolDown, 0x24);
			/// @brief 默认冷却时间
			INT_PROPERTY(CoolDownInterval, __get_CoolDownInterval, __set_CoolDownInterval, 0x28);
			/// @brief 卡牌编号
			INT_READONLY_PROPERTY(Index, __get_Index, 0x2C);
			/// @brief X 坐标偏移量
			INT_PROPERTY(XInConveyorBelt, __get_XInConveyorBelt, __set_XInConveyorBelt, 0x30);
			/// @brief 卡牌类型
			T_PROPERTY(SeedType::SeedType, ContentCard, __get_ContentCard, __set_ContentCard, 0x34);
			/// @brief 卡牌模仿的植物类型
			T_PROPERTY(SeedType::SeedType, ContentCardImitative, __get_ContentCardImitative, __set_ContentCardImitative, 0x38);
			/// @brief 老虎机倒计时
			INT_PROPERTY(SlotCountdown, __get_SlotCountdown, __set_SlotCountdown, 0x3C);
			/// @brief 老虎机中，下一个卡牌的类型
			T_PROPERTY(SeedType::SeedType, SlotType, __get_SlotType, __set_SlotType, 0x40);
			/// @brief 老虎机中的位置
			T_PROPERTY(FLOAT, SlotPosition, __get_SlotPosition, __set_SlotPosition, 0x44);
			/// @brief 该卡槽是否可点击
			T_PROPERTY(BOOLEAN, Enable, __get_Enable, __set_Enable, 0x48);
			/// @brief 该卡槽是否正在CD
			T_PROPERTY(BOOLEAN, Active, __get_Active, __set_Active, 0x49);
			INT_PROPERTY(UsageCount, __get_UsageCount, __set_UsageCount, 0x4C);
			/// @brief 该卡槽进入CD.
			/// @param duration 冷却时间。若为 -1，则改该卡槽的默认冷却时间
			void EnterCoolDown(int duration = -1);
			/// @brief 设置卡牌类型
			/// @param type 卡牌类型
			/// @param imitater_type 模仿者模仿的植物类型s 
			void SetPacketType(SeedType::SeedType type, SeedType::SeedType imitater_type = SeedType::None);
		};
		/// @brief 获得指定卡槽卡片
		/// @param index 卡片下标
		/// @return 指定的卡槽卡片
		PVZ::CardSlot::SeedCard GetCard(int index);
		using SeedPacket = SeedCard;
	};
	using SeedBank = CardSlot;
	/// @brief 存储小游戏、解谜、生存等特殊模式的数据的类。
	class Challenge : public BaseClass
	{
	public:
		Challenge(int address);
		static const int MemSize = 0x0BC;
		/// @brief 调整该类在 PVZ 中对象的大小。
		/// @note 请在派生类中调用这个函数。
		/// @note 调用该函数后，新生成的存档与原版存档不兼容，请注意清理。
		/// @param MemSize 更改后的大小。
		/// @return 是否修改成功。
		static bool SetMemSize(int NewSize);

		/// @brief 是否在拖拽植物
		T_READONLY_PROPERTY(BOOLEAN, DragingPlant, __get_DragingPlant, 8);
		/// @brief 宝石迷阵类模式中，拖拽的 X 格位
		INT_READONLY_PROPERTY(DragingX, __get_DragingX, 0xC);
		/// @brief 宝石迷阵类模式中，拖拽的 Y 格位
		INT_READONLY_PROPERTY(DragingY, __get_DragingY, 0x10);
		/// @brief 检测指定位置是否有宝石迷阵模式的弹坑
		/// @param row 行
		/// @param column 列
		/// @return 指定位置是否有弹坑
		BOOLEAN HaveCrater(int row, int column);
		/// @brief 设置指定位置是否有宝石迷阵模式的弹坑
		/// @param row 行
		/// @param column 列
		/// @param b 指定位置是否将有弹坑
		void SetCrater(int row, int column, BOOLEAN b);
		/// @brief 是否升级双发
		T_PROPERTY(BOOLEAN, UpgradedRepeater, __get_UpgradedRepeater, __set_UpgradedRepeater, 0x4A);
		/// @brief 是否升级大喷菇
		T_PROPERTY(BOOLEAN, UpgradedFumeshroom, __get_UpgradedFumeshroom, __set_UpgradedFumeshroom, 0x4B);
		/// @brief 是否升级高坚果
		T_PROPERTY(BOOLEAN, UpgradedTallnut, __get_UpgradedTallnut, __set_UpgradedTallnut, 0x4C);
		/// @brief 宝石迷阵模式中，自移动植物起连续达成的消除次数。
		INT_PROPERTY(BeghouledMatchesThisMove, __get_BeghouledMatchesThisMove, __set_BeghouledMatchesThisMove, 0x50);
		/// @brief 小游戏状态
		/// @see ChallengeState
		T_PROPERTY(ChallengeState::ChallengeState, State, __get_State, __set_State, 0x54);
		/// @brief 状态倒计时
		INT_PROPERTY(AttributeCountdown, __get_AttributeCountdown, __set_AttributeCountdown, 0x58);
		/// @brief 传送带传送下一张卡的倒计时
		INT_PROPERTY(ConveyorCountdown, __get_ConveyorCountdown, __set_ConveyorCountdown, 0x5C);
		/// @brief 关卡进程
		INT_PROPERTY(LevelProcess, __get_LevelProcess, __set_LevelProcess, 0x60);
		/// @brief 是否绘制保龄球关卡的红线
		T_PROPERTY(BOOLEAN, ShowBowlingLine, __get_ShowBowlingLine, __set_ShowBowlingLine, 0x64);
		/// @brief 传送带最近一次传送的卡牌类型
		T_PROPERTY(SeedType::SeedType, ConveyorLastCard, __get_ConveyorLastCard, __set_ConveyorLastCard, 0x68);
		/// @brief 关卡轮数
		INT_PROPERTY(Round, __get_Round, __set_Round, 0x6C);
		/// @brief 老虎机使用次数
		INT_PROPERTY(SlotMachineRollCount, __get_SlotMachineRollCount, __set_SlotMachineRollCount, 0x70);
		INT_PROPERTY(ChallengeGridX, __get_ChallengeGridX, __set_ChallengeGridX, 0x0A8);
		INT_PROPERTY(ChallengeGridY, __get_ChallengeGridY, __set_ChallengeGridY, 0x0AC);
		/// @brief 初始罐子数量
		INT_READONLY_PROPERTY(VaseCount, __get_VaseCount, 0x0B0);
		/// @brief 下雨相关的倒计时
		INT_PROPERTY(RainCounter, __get_RainCounter, __set_RainCounter, 0x0B4);
		/// @brief 智慧树话语序号
		INT_READONLY_PROPERTY(TreeOfWisdomTalkIndex, __get_TreeOfWisdomTalkIndex, 0x0B8);

		/// @brief 压扁指定的脑子，并计算因此产生的得分
		/// @param brain 将被压扁的脑子
		/// @see IZBrain
		void IZSquishBrain(IZBrain brain);
		/// @brief 在指定行随机种植若干个指定类型的植物，并对其进行 IZ 关卡的特殊调整。
		/// @param type 植物类型
		/// @param count 种植数量
		/// @param row 指定的行。若为 -1，则没有行限制。
		void IZPlacePlants(SeedType::SeedType type, int count, int row);
	};
	using Miscellaneous = Challenge;

#pragma endregion

#pragma region methods

	void InitImages();
	Mouse GetMouse();
	//若 BaseAddress 为 0，返回空指针
	Board GetBoard();

#pragma endregion

#pragma region Images

	class Resource
	{
	public:
		static Image* IMAGE_BLANK;
	};

#pragma endregion
};