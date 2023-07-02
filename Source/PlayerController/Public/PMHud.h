#pragma once

#include "CoreMinimal.h"
#include "OnScreenDebugger.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "PMHud.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMHud : public AHUD
{
	GENERATED_BODY()
public:
private:
protected:
	virtual void DrawHUD() override;
};
