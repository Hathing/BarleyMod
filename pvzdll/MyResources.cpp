#include "MyResources.hpp"

// 初始化资源地址
MyImage IMAGE_TEST = 0;

/// @brief 导入图片资源
MyImage ImportImage(const char* file_path, const PVZ::PVZApp& app)
{
	auto path = Draw::ToString(file_path);
	MyImage img = (MyImage)PVZ::Memory::Execute(AsmBuilder()
		.push(00).push_imm32(path)
		.mov_reg_imm(REG_ECX, app.GetBaseAddress())
		.invoke(0x5548E0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
	DestroyPString(path);
	return img;
}

// 导入新资源总函数
void ImportNewResources(PVZ::PVZApp& app)
{
	//导入贴图
	IMAGE_TEST = ImportImage("./newimages/04-ProjectileCactus.png", app);
}