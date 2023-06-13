#include "PlayerMovement.h"

UPlayerMovement::UPlayerMovement()
{
    m_maxwalkspeed = 500.f;
    m_maxairspeed  = 2000.f;
    m_acelerationspeed = 2400.f;
	m_friction = 1000.f;
}

void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	m_tickcheck = false;

	FString velstr = FString::SanitizeFloat(Velocity.Size2D());
	OnScreenDebugger::DrawDebugMessage("vel: " + velstr, FColor::White, 10);
	OnScreenDebugger::DrawDebugMessage("onground", IsMovingOnGround(), 11);
}

void UPlayerMovement::TickVelocity(float delta)
{
	// TODO : shit hack
	if (m_tickcheck) 
	{
		OnScreenDebugger::DrawDebugMessage("tc", FColor::Red, 91);
		return; 
	}
	m_tickcheck = true;

	if (IsMovingOnGround())
	{
		UpdateVelocityGround(delta);
	}
	else
	{
		UpdateVelocityAir(delta);
	}
}

void UPlayerMovement::UpdateVelocity(FVector moveadd)
{
	const auto& addstr = FString::SanitizeFloat(moveadd.Size2D());
	OnScreenDebugger::DrawDebugMessage("addspeed: " + addstr, FColor::Green, 80);

	float wishvel = (Velocity + moveadd).Size2D();

	if (wishvel <= m_maxwalkspeed)
	{ Velocity += moveadd; }
	else { OnScreenDebugger::DrawDebugMessage("clamped", FColor::Green, 81); }
}

void UPlayerMovement::UpdateVelocityGround(float delta)
{
	// wish
	FVector wishdir = m_wishdir.GetSafeNormal2D();
	float wishspeed = m_maxwalkspeed;

	// fpply friction
	float speed = Velocity.Size2D();
	if (speed != 0)
	{
		if (speed < 1.f)
		{
			Velocity = { 0, 0, Velocity.Z };
			return;
		}
		float newspeed = FMath::Max(0, speed - m_friction * delta);
		newspeed /= speed;

		Velocity *= newspeed;
	}

	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, m_acelerationspeed * delta);

	// update speed
	UpdateVelocity(wishdir * addspeed);
}

void UPlayerMovement::UpdateVelocityAir(float delta)
{
	FVector wishvel = m_wishdir;
	FVector wishdir = wishvel.GetSafeNormal2D();

	// get updated speed
	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(m_maxairspeed - currentspeed, 0, m_acelerationspeed * delta);

	// update speed
	UpdateVelocity(wishdir * addspeed);
}

void UPlayerMovement::CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration)
{
	TickVelocity(delta);
}

void UPlayerMovement::ApplyVelocityBraking(float delta, float friction, float brakingDeceleration)
{
    float speed = Velocity.Size2D();

    if (speed <= 0.15f || !HasValidData() || HasAnimRootMotion() || delta < MIN_TICK_TIME)
	{
        return;
    }

    brakingDeceleration = FMath::Max(brakingDeceleration, speed);

    if (FMath::IsNearlyZero(friction) || FMath::IsNearlyZero(brakingDeceleration))
	{
        return;
    }

    FVector oldVel = Velocity;
    FVector revAccel = friction * brakingDeceleration * oldVel.GetSafeNormal();

    Velocity -= revAccel * delta;

    if ((Velocity | oldVel) <= 0.f || FMath::IsNearlyZero(Velocity.SizeSquared(), KINDA_SMALL_NUMBER))
	{
        Velocity = FVector::ZeroVector;
    }
}

bool UPlayerMovement::DoJump(bool bClientSimulation)
{
    return Super::DoJump(bClientSimulation);
}

void UPlayerMovement::TwoWallAdjust(FVector& delta, const FHitResult& hit, const FVector& oldHitNormal) const
{
    Super::Super::TwoWallAdjust(delta, hit, oldHitNormal);
}

FVector UPlayerMovement::HandleSlopeBoosting(const FVector& slideResult, const FVector& delta, const float time, const FVector& normal, const FHitResult& hit) const
{
    return slideResult;
}

float UPlayerMovement::SlideAlongSurface(const FVector& delta, float time, const FVector& normal, FHitResult& hit, bool bHandleImpact)
{
    return Super::Super::SlideAlongSurface(delta, time, normal, hit, bHandleImpact);
}

float UPlayerMovement::GetMaxSpeed() const
{
    return IsMovingOnGround() ? m_maxwalkspeed : m_maxairspeed;
}
