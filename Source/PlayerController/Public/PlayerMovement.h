#pragma once

#include "CoreMinimal.h"

#include "OnScreenDebugger.h"

#include "GameFramework/PhysicsVolume.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "PlayerMovement.generated.h"

/**
 * does all movement calculations
 */
UCLASS()
class TowerDefence_API UPlayerMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	UPlayerMovement();

protected:

private:
	// veloicty methods
	void TickVelocity(float delta);

	void UpdateVelocity(FVector moveadd);
	void UpdateVelocityGround(float delta);
	void UpdateVelocityAir(float delta);
	void DoFriction(float delta);

	// unit vector of direction player wants to move
	FVector m_wishdir;
	bool m_tickcheck;

public:
	inline void UpdateWishDir(FVector wishdir)
	{
		OnScreenDebugger::DrawDebugMessage("u", FColor::Green, 65);
		m_wishdir = wishdir; 
		if (wishdir == FVector::Zero())
		{
			OnScreenDebugger::DrawDebugMessage("zero", FColor::Red, 92);
		} 
	}

	// for debug use TODO : move or make priavte on final build
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float MaxGroundSpeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float MaxAirSpeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float GroundAccelerationSpeed;
	
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float AirAccelerationSpeed;
	
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float BaseGroundFriction;
	
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float JumpSpeed;

	// override methods
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration) override;

	bool DoJump(bool bClientSimulation) override;
	float GetMaxSpeed() const override;

};