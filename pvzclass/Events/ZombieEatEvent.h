#pragma once
#include "DLLEvent.h"

// 僵尸啃食植物事件
// 参数：触发事件的僵尸和植物
// 无返回值
class ZombieEatEvent : public DLLEventTemplate<0x52FB40, 7, REG_ECX, REG_EDI>
{
public:
	ZombieEatEvent() : DLLEventTemplate() { Init("onZombieEat"); };
};