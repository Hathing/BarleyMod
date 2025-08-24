#include "pch.h"

byte __asm__PlayCollectSound[19]
{
	MOV_EDX(0),
	INVOKE(0x432B00),
	RET
};

void PlayCollectSound(PVZ::Coin coin)
{
	SETARG(__asm__PlayCollectSound, 1) = coin.GetBaseAddress();
	PVZ::Memory::Execute(STRING(__asm__PlayCollectSound));
}

bool onCoinAutoCollect(PVZ::Coin coin)
{
	if (coin.ExistedTime > 100 && coin.Collected == false && (int)coin.Type <= 3)
	{
		coin.Collect();
		//PlayCollectSound(coin);
	}
	return coin.Collected;
}

void InitCoinEvents()
{
	PVZEvent::CoinAutoCollectEvent((int)onCoinAutoCollect);
}