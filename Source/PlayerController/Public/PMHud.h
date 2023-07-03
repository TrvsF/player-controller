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
	void DrawCircle(FVector2D pos, float radius, FLinearColor colour);
	void DrawCircle(float posx, float posy, float radius, FLinearColor colour);
protected:
	virtual void DrawHUD() override;
};
