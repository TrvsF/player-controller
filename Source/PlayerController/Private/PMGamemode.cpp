#include "PMGamemode.h"

APMGamemode::APMGamemode()
{
	HUDClass = APMHud::StaticClass();
}

void APMGamemode::StartPlay()
{
	Super::StartPlay();
}

void APMGamemode::HandleStartingNewPlayer_Implementation(ATowerDefence* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
