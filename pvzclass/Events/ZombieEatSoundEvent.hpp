#pragma once
#include "DLLEvent.h"

/// @brief 播放僵尸啃食音效的事件。
/// @note 魅惑菇和大蒜在此时机发挥作用。
/// @param 依次为：触发事件的僵尸、被其啃食的植物。
/// @return 是否执行原版对该事件的结算。
class ZombieEatSoundEvent : public DLLEventTemplate<0x52B964, 6, 0x52BAD7, REG_ESI, REG_EDI>
{
public:
	ZombieEatSoundEvent() : DLLEventTemplate() { Init("onZombieEatSound"); };
	ZombieEatSoundEvent(const char* str) : DLLEventTemplate() { Init(str); };
	ZombieEatSoundEvent(int address) : DLLEventTemplate() { Init(address); };
};
