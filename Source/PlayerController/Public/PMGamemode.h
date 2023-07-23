#pragma once

#include "CoreMinimal.h"
#include "OnScreenDebugger.h"
#include "PMHud.h"
#include "GameFramework/GameModeBase.h"
#include "PMGamemode.generated.h"

UCLASS()
class TowerDefence_API APMGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APMGamemode();

	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(ATowerDefence* NewPlayer) override;
};
