#pragma once
#include "MyClasses.hpp"

class MyImage : public PVZ::BaseClass
{
public:
	MyImage(int address) : PVZ::BaseClass(address) {};

	/// @brief 导入图片资源
	/// @param file_path 文件相对路径
	/// @param app LawnApp
	void ImportImage(const char* file_path, const PVZ::PVZApp& app);
};

void ImportNewResources(PVZ::PVZApp& app);

namespace NewImage
{
	extern MyImage TEST;
	extern MyImage PLANT_LEVELS[6];
	extern MyImage HP_BAR_BASE;
	extern MyImage HP_BAR_BORDER;
	extern MyImage ICICLE;// 冰锥
}