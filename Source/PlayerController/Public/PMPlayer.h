#pragma once

#include "CoreMinimal.h"

#include "PMCharacter.h"
#include "PMRocket.h"
#include "OnScreenDebugger.h"

#include "GameFramework/Controller.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

#include "Camera/CameraComponent.h"

#include "PMPlayer.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMPlayer : public APMCharacter
{
	GENERATED_BODY()

public:
	APMPlayer(const FObjectInitializer& objectInitializer);

	UPROPERTY(Category = "Player Rocket", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ARocket> Rocket;

	UPROPERTY(Category = "Player View", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		float YawSensMultiplyer;

	UPROPERTY(Category = "Player View", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		float PitchSensMultiplyer;

	UPROPERTY(Category = "Player View", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComp;
	
	UPROPERTY(Category = "Player View", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* FirstPersonViewmodel;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	

protected:
	virtual void BeginPlay() override;

	void Shoot();
	void Jump() override;
	void Move  (float Value);
	void Strafe(float Value);

	void AddControllerYawInput  (float Value) override;
	void AddControllerPitchInput(float Value) override;
};