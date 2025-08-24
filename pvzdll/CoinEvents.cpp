#include "pch.h"
void onCoinAutoCollect(PVZ::Coin coin)
{
	if (coin.ExistedTime > 100 && coin.Collected == false)
	{
		coin.Collected = true;
		//Creator::CreateSound();
	}
	return;
}

void InitCoinEvents()
{
	PVZEvent::CoinAutoCollectEvent((int)onCoinAutoCollect);
}