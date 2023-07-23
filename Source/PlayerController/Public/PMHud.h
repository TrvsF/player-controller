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

	UPROPERTY(Category = "XHair", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		int XHairGap;

	virtual void Tick(float DeltaTime) override;


private:
	bool drawcircle;
	bool drawcross;
	float scaleoffset;

	void DrawCircle(FVector2D pos, float radius, FLinearColor colour);
	void DrawCircle(float posx, float posy, float radius, FLinearColor colour);

	void DrawXHair();
protected:
	virtual void DrawHUD() override;
};
