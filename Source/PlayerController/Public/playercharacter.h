// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerMovement.h"

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

	void Jump() override;
	virtual void StopJumping() override;
	// virtual void OnJumped_Implementation() override;
	// virtual bool CanJumpInternal_Implementation() const override;

	void AddControllerYawInput(float Value) override;
	void AddControllerPitchInput(float Value) override;

private:
	UPROPERTY(Category = "Camera Comp", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComp;

	UPROPERTY(Category = "Player Comp", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	UPROPERTY(Category = "Player Comp", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;

	UPlayerMovement* MovementPtr;

	void DrawDebugMessage(char* message,   FColor colour, int id);
	void DrawDebugMessage(FString message, FColor colour, int id);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward (float Value);
	void StrafeRight (float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
