// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

void init()
{
	PVZ::Memory::localExecute = true;
	PVZ::Memory::DLLAddress = (int)GetModuleHandle(TEXT("BarleyMod.dll"));

	if (!PVZ::Memory::DLLAddress)
	{
		MessageBox(NULL, TEXT("改版模块加载失败！"), TEXT("Error"), MB_ICONERROR);
		exit(-1);
	}

	PVZ::Memory::Variable = PVZ::Memory::AllocMemory(4);

	Creator::AsmInit();
}