#include "MyResources.hpp"
#include <cassert>

void MyImage::ImportImage(const char* file_path, const PVZ::PVZApp& app)
{
	auto path = Draw::ToString(file_path);
	int img_addr = PVZ::Memory::Execute(AsmBuilder()
		.push(00).push_imm32(path)
		.mov_reg_imm(REG_ECX, app.GetBaseAddress())
		.invoke(0x5548E0)
		.mov_mem_reg(PVZ::Memory::Variable, REG_EAX)
		.ret()
	);
	assert(img_addr == 0);
	DestroyPString(path);
	this->BaseAddress = img_addr;
}

namespace NewImage
{
	MyImage TEST{ NULL };
	MyImage PLANT_LEVELS[6]{ {NULL},{NULL},{NULL},{NULL},{NULL},{NULL} };
	MyImage HP_BAR_BASE{ NULL };
	MyImage HP_BAR_BORDER{ NULL };
}

// 导入新资源总函数
void ImportNewResources(PVZ::PVZApp& app)
{
	//导入贴图
	NewImage::TEST.ImportImage("./newimages/04-ProjectileCactus.png", app);
	NewImage::PLANT_LEVELS[0].ImportImage("./newimages/18-0.png", app);
	NewImage::PLANT_LEVELS[1].ImportImage("./newimages/18-1.png", app);
	NewImage::PLANT_LEVELS[2].ImportImage("./newimages/18-2.png", app);
	NewImage::PLANT_LEVELS[3].ImportImage("./newimages/18-3.png", app);
	NewImage::PLANT_LEVELS[4].ImportImage("./newimages/18-4.png", app);
	NewImage::PLANT_LEVELS[5].ImportImage("./newimages/18-5.png", app); 
	NewImage::HP_BAR_BASE.ImportImage("./newimages/20-\xD1\xAA\xCC\xF5\xB5\xD7.png", app);
	NewImage::HP_BAR_BORDER.ImportImage("./newimages/21-\xD1\xAA\xCC\xF5\xBF\xF2.png", app);
}