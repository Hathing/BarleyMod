#pragma once
#include "DLLEvent.h"

// Zombie 行为动作的更新。
// 时机上先于原版的更新。
// 参数：更新的 Zombie
// 无返回值
class ZombieUpdateActionEvent : public DLLEvent
{
public:
	ZombieUpdateActionEvent(int address);
};

ZombieUpdateActionEvent::ZombieUpdateActionEvent(int address)
{
	hookAddress = 0x52B112;
	rawlen = 6;
	BYTE code[] = { PUSH_EAX, INVOKE(address), ADD_ESP(4) };
	start(STRING(code));
}
