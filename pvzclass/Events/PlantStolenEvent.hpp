#pragma once
#include "DLLEvent.h"

// 植物被偷走事件。
// 无返回值
/// @param 依次为：触发事件的植物。
class PlantStolenEvent : public DLLEvent
{
public:
	PlantStolenEvent(int address);
};

PlantStolenEvent::PlantStolenEvent(int address)
{
	hookAddress = 0x5304B6;
	rawlen = 7;
	BYTE code[] =
	{
		PUSH_EAX,
		INVOKE(address),
		ADD_ESP(4),
	};
	start(STRING(code));
}