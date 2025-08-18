#pragma once
#include "DLLEvent.h"

namespace PVZEvent
{
	/// @brief 僵尸被施加黄油效果的事件
	/// @param 触发事件的僵尸
	/// @note 与 ZombieButterEvent 不同，免疫黄油的单位不会触发此事件
	class ZombieApplyButterEvent : public DLLEventTemplate<0x5327D4, 5, REG_ESI>
	{
	public:
		ZombieApplyButterEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieApplyButterEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieApplyButterEvent() : ZombieApplyButterEvent("onZombieApplyButter") {};
	};

	/// @brief 僵尸被冻结的事件
	/// @param 触发事件的僵尸
	/// @note 与 ZombieFrozeEvent 不同，免疫冻结的单位不会触发此事件
	class ZombieHitIceTrapEvent : public DLLEventTemplate<0x53249E, 5, REG_ESI>
	{
	public:
		ZombieHitIceTrapEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieHitIceTrapEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieHitIceTrapEvent() : ZombieHitIceTrapEvent("onZombieHitIceTrap") {};
	};

	/// @brief 僵尸被减速的事件
	/// @param 触发事件的僵尸
	/// @note 与 ZombieDecelerateEvent 不同，免疫减速的单位不会触发此事件
	class ZombieChillEvent : public DLLEventTemplate<0x5309E4, 5, REG_ESI>
	{
	public:
		ZombieChillEvent(const char* str) : DLLEventTemplate() { Init(str); };
		ZombieChillEvent(int address) : DLLEventTemplate() { Init(address); };
		ZombieChillEvent() : ZombieChillEvent("onZombieChill") {};
	};
}
