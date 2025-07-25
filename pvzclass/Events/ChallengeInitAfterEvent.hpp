#pragma once
#include "DLLEvent.h"

/// @brief 关卡初始化后的事件。
/// @param 触发事件的 Challenge
class ChallengeInitAfterEvent : public DLLEventTemplate<0x41F6D8, 5, REG_EDI>
{
public:
	ChallengeInitAfterEvent(const char* str) : DLLEventTemplate() { Init(str); };
	ChallengeInitAfterEvent(int address) : DLLEventTemplate() { Init(address); };
	ChallengeInitAfterEvent() : DLLEventTemplate() { Init("onChallengeInitAfter"); };
};