#pragma once

#include "CoreMinimal.h"

#include "OnScreenDebugger.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"

#include "PlayerMovement.generated.h"

/**
 * 
 */
UCLASS()
class PLAYERCONTROLLER_API UPlayerMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	UPlayerMovement();

protected:

private:
	void DoMovement(float delta);

	// unit vector of direction player wants to move
	FVector GetWishDir();

	FVector m_localvelocity;

	FVector m_maxwalkspeedvec;
	FVector m_maxairspeedvec;

	int m_fwdvalue = 0;
	int m_rgtvalue = 0;

public:
	inline void AddFowardInput(float Value)
	{
		// xor for 0 or 1, then make neg/pos
		int bit = (int)fabsf(Value);
		int newValue = (m_fwdvalue ^ bit) * Value;
		
		m_fwdvalue = newValue;
	}

	inline void AddStrafeInput(float Value)
	{
		// xor for 0 or 1, then make neg/pos
		int bit = (int)fabsf(Value);
		int newValue = (m_rgtvalue ^ bit) * Value;

		m_rgtvalue = newValue;
	}

	// for debug use TODO : move or make priavte on final build
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_maxwalkspeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_maxairspeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_acelerationspeed;

	// override methods
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration) override;
	void ApplyVelocityBraking	(float delta, float friction, float brakingDeceleration) override;
	void TwoWallAdjust			(FVector& delta, const FHitResult& hit, const FVector& oldHitNormal) const override;
	
	FVector HandleSlopeBoosting	(const FVector& slideResult, const FVector& delta, const float time, const FVector& normal, const FHitResult& hit) const override;
	bool	ShouldCatchAir		(const FFindFloorResult& oldFloor, const FFindFloorResult& newFloor) override;
	bool	DoJump				(bool bClientSimulation) override;
	float	SlideAlongSurface	(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact = false) override;
	float	GetMaxSpeed() const override;

};