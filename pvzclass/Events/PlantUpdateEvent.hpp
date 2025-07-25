#pragma once
#include "DLLEvent.h"

namespace PVZEvent
{
	/// @brief 植物更新事件。
	/// @param 触发事件的植物。
	/// @return 是否考虑进行此次更新。
	class PlantUpdateEvent : public DLLEvent
	{
	public:
		PlantUpdateEvent();
	};

	PlantUpdateEvent::PlantUpdateEvent()
	{
		int procAddress = PVZ::Memory::GetProcAddress("onPlantUpdate");
		hookAddress = 0x463E43;
		rawlen = 6;
		BYTE code[] =
		{
			PUSH_EBX,
			INVOKE(procAddress),
			ADD_ESP(4),

			TEST_AL_AL,
			JNZ(8),
			POPAD,
			MOV_ECX(0x463EE8),
			JMP_REG32(REG_ECX)
		};
		start(STRING(code));
	}
}