// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerMovement.h"
#include "OnScreenDebugger.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

#include "Camera/CameraComponent.h"

#include "PlayerCharacter.generated.h"

class UPlayerMovement;

UCLASS()
class PLAYERCONTROLLER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& objectInitializer);

	inline FVector GetMovementVector() const
	{ return m_movementvector; }

	inline FVector GetWishDir() const
	{
		bool onground = m_movementptr->IsMovingOnGround();
		float ffactor = onground ? 1.f : 0.1f;
		float rfactor = onground ? 1.f : 0.2f;

		const auto& f = GetActorForwardVector() * m_movementvector.X * ffactor;
		const auto& r = GetActorRightVector()   * m_movementvector.Y * rfactor;

		return f + r;
	}

private:
	UPROPERTY(Category = "Player Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComp;

	UPROPERTY(Category = "Player Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		float YawSensMultiplyer;

	UPROPERTY(Category = "Player Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		float PitchSensMultiplyer;

	UPROPERTY(Category = "Player Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	UPROPERTY(Category = "Player Collision", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;

	UPlayerMovement* m_movementptr;
	FVector m_movementvector;

protected:
	virtual void BeginPlay() override;

	void Jump() override;
	void Move  (float Value);
	void Strafe(float Value);

	void AddControllerYawInput  (float Value) override;
	void AddControllerPitchInput(float Value) override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnMovementModeChanged(EMovementMode prevMode, uint8 prevCustomMode) override;
};