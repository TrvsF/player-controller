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
	APMHud();

	UPROPERTY(Category = "XHair", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		int XHairSize;

	UPROPERTY(Category = "XHair", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		int XHairThickness;
private:
protected:
	virtual void DrawHUD() override;
};
