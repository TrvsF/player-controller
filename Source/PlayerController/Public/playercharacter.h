// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

#include "Camera/CameraComponent.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void Jump() override;
	virtual void StopJumping() override;
	// virtual void OnJumped_Implementation() override;
	// virtual bool CanJumpInternal_Implementation() const override;

	inline float GetLastJumpTime() const
	{ return LastJumpTime; }

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Player")
		UStaticMeshComponent* MeshComp;

	void DrawDebugMessage(char* message, FColor colour, int id);
	void DrawDebugMessage(FString message, FColor colour, int id);

	/** when we last jumped */
	float LastJumpTime;

	/** throttle jump boost when going up a ramp, so we don't spam it */
	float LastJumpBoostTime;

	/** maximum time it takes to jump */
	float MaxJumpTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward (float Value);
	void StrafeRight (float Value);
	void TurnAtRate  (float Value);
	void LookUpAtRate(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpAtRate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
