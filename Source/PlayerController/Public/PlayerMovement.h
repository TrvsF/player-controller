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
	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float WalkSpeed;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float GroundAcceleration;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float AirAcceleration;

	UPROPERTY(Category = "Player Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float AirSpeedCap;

private:
	void PreemptCollision(float delta);

	bool bLastGrounded = false;

public:
	FVector WishDir;

	void CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration) override;
	void ApplyVelocityBraking(float delta, float friction, float brakingDeceleration) override;

	bool DoJump(bool bClientSimulation) override;

	void TwoWallAdjust(FVector& delta, const FHitResult& hit, const FVector& oldHitNormal) const override;
	FVector HandleSlopeBoosting(const FVector& slideResult, const FVector& delta, const float time, const FVector& normal, const FHitResult& hit) const override;
	bool ShouldCatchAir(const FFindFloorResult& oldFloor, const FFindFloorResult& newFloor) override;

	float SlideAlongSurface(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact = false);

	float GetMaxSpeed() const override;

};