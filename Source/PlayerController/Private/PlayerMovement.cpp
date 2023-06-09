#include "PlayerMovement.h"

UPlayerMovement::UPlayerMovement()
{
    m_maxwalkspeed = 800.f;
    m_maxairspeed = 1500.f;
    m_accelerationspeed = 400000.f;

    m_maxwalkspeedvec = FVector{ m_maxwalkspeed, m_maxwalkspeed, 0 };
    m_maxairspeedvec  = FVector{ m_maxwalkspeed, m_maxwalkspeed, 0 };
}

void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    CalcVelocity(DeltaTime, GetMaxBrakingDeceleration(), false, false);
}

void UPlayerMovement::CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration)
{
    // validate
    if (!HasValidData() || HasAnimRootMotion() || delta < MIN_TICK_TIME || (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy))
	{
        return;
    }

    // setup vars
    bool ismovingonground = IsMovingOnGround();

    // firction
    friction = FMath::Max(0.f, friction);
    if (ismovingonground && GetWishDir() == FVector::Zero())
    {
        const float actualBrakingFriction = bUseSeparateBrakingFriction ? BrakingFriction : friction;
        ApplyVelocityBraking(delta, actualBrakingFriction, brakingDeceleration);
    }

    const auto& current_speed = FVector::DotProduct(Velocity, GetWishDir());
    const auto& add_speed = FMath::Clamp(m_maxwalkspeed - current_speed, 0, m_accelerationspeed * delta);
    
    Velocity += GetWishDir() * add_speed;

    const auto& WishDirStr = GetWishDir().ToString();
    OnScreenDebugger::DrawDebugMessage("wish:  " + WishDirStr, FColor::White, 6);

    const auto& CurrentSpeedStr = FString::SanitizeFloat(current_speed);
    OnScreenDebugger::DrawDebugMessage("current speed:  " + CurrentSpeedStr, FColor::White, 7);

    const auto& AddSpeedStr = FString::SanitizeFloat(add_speed);
    OnScreenDebugger::DrawDebugMessage("add speed:  " + AddSpeedStr, FColor::White, 8);

    m_fwdvalue = 0;
    m_rgtvalue = 0;
}

void UPlayerMovement::ApplyVelocityBraking(float delta, float friction, float brakingDeceleration)
{
    float speed = Velocity.Size2D();

    if (speed <= 0.1f || !HasValidData() || HasAnimRootMotion() || delta < MIN_TICK_TIME)
	{
        return;
    }

    friction *= BrakingFrictionFactor;
    brakingDeceleration = FMath::Max(brakingDeceleration, speed);

    if (FMath::IsNearlyZero(friction) || FMath::IsNearlyZero(brakingDeceleration))
	{
        return;
    }

    const FVector oldVel = Velocity;
    const FVector revAccel = friction * brakingDeceleration * oldVel.GetSafeNormal();

    Velocity -= revAccel * delta;

    if ((Velocity | oldVel) <= 0.f || FMath::IsNearlyZero(Velocity.SizeSquared(), KINDA_SMALL_NUMBER))
	{
        Velocity = FVector::ZeroVector;
    }
}

bool UPlayerMovement::DoJump(bool bClientSimulation)
{
    return bCheatFlying || Super::DoJump(bClientSimulation);
}

void UPlayerMovement::TwoWallAdjust(FVector& delta, const FHitResult& hit, const FVector& oldHitNormal) const
{
    Super::Super::TwoWallAdjust(delta, hit, oldHitNormal);
}

FVector UPlayerMovement::HandleSlopeBoosting(const FVector& slideResult, const FVector& delta, const float time, const FVector& normal, const FHitResult& hit) const
{
    return slideResult;
}

bool UPlayerMovement::ShouldCatchAir(const FFindFloorResult& oldFloor, const FFindFloorResult& newFloor)
{
    float speed = Velocity.Size2D();
    float maxSpeed = m_maxwalkspeed *1.5f;

    float speedMult = maxSpeed / speed;
    float nrmDiff = newFloor.HitResult.ImpactNormal.Z - oldFloor.HitResult.ImpactNormal.Z;

    // Catch air if the difference between the old floor normals & new floor normals is something
    // and the player's speed is low
    return (nrmDiff > 0.f && speedMult < 0.5f) || Super::ShouldCatchAir(oldFloor, newFloor);
}

float UPlayerMovement::SlideAlongSurface(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact)
{
    return Super::Super::SlideAlongSurface(delta, time, normal, hit, bHandleImpact);
}

float UPlayerMovement::GetMaxSpeed() const
{
    return m_maxwalkspeed;
}
