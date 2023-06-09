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
	

	// unit vector of direction player wants to move
	FVector m_wishdir;

	FVector m_maxwalkspeedvec;
	FVector m_maxairspeedvec;

public:
	int m_fwdvalue = 0;
	int m_rgtvalue = 0;

	inline void AddInput(FVector dirvec)
	{
		m_wishdir += dirvec;
		dirvec = dirvec.GetSafeNormal(0.01f); // never have to piss around
	}

	inline FVector GetWishDir()
	{
		const auto& player = GetPawnOwner();

		FVector inputvec = FVector(m_fwdvalue, m_rgtvalue, 0);
		FVector wishdir  = player->GetActorRotation().RotateVector(inputvec);
		
		return  wishdir.GetSafeNormal(0.01f);
	}

	// for debug use TODO : dont compile on final build
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_maxwalkspeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_maxairspeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float m_accelerationspeed;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration) override;
	void ApplyVelocityBraking(float delta, float friction, float brakingDeceleration) override;

	bool DoJump(bool bClientSimulation) override;

	void TwoWallAdjust(FVector& delta, const FHitResult& hit, const FVector& oldHitNormal) const override;
	FVector HandleSlopeBoosting(const FVector& slideResult, const FVector& delta, const float time, const FVector& normal, const FHitResult& hit) const override;
	bool ShouldCatchAir(const FFindFloorResult& oldFloor, const FFindFloorResult& newFloor) override;

	float SlideAlongSurface(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact = false);

	float GetMaxSpeed() const override;

};