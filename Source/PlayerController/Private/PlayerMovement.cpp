#include "PlayerMovement.h"

UPlayerMovement::UPlayerMovement()
{
    m_maxgroundspeed = 700.f;
    m_maxairspeed  = 2000.f;

    m_acelerationgroundspeed = 3200.f;
    m_acelerationairspeed = 100.f;

	m_friction = 1500.f;
}

void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	m_tickcheck = false;

	FString velstr = FString::SanitizeFloat(Velocity.Size2D());
	OnScreenDebugger::DrawDebugMessage("vel: " + velstr, FColor::White, 100);
	OnScreenDebugger::DrawDebugMessage("onground", IsMovingOnGround(), 101);
}

void UPlayerMovement::TickVelocity(float delta)
{
	// TODO : shit hack
	if (m_tickcheck) 
	{
		OnScreenDebugger::DrawDebugMessage("tc", FColor::Red, 102);
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
	OnScreenDebugger::DrawDebugMessage("addspeed: " + addstr, FColor::Green, 103);

	bool onground = IsMovingOnGround();
	float wishvel = (Velocity + moveadd).Size2D();

	if (wishvel <= (onground ? m_maxgroundspeed : m_maxairspeed))
	{ 
		Velocity += moveadd;
	}
	// debug
	else 
	{ 
		OnScreenDebugger::DrawDebugMessage("clamped", FColor::Green, 104);
	}
}

void UPlayerMovement::UpdateVelocityGround(float delta)
{
	// wish
	FVector wishdir = m_wishdir.GetSafeNormal2D();
	float wishspeed = m_maxgroundspeed;

	DoFriction(delta);

	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, m_acelerationgroundspeed * delta);

	// update speed
	UpdateVelocity(wishdir * addspeed);
}

void UPlayerMovement::UpdateVelocityAir(float delta)
{
	// wish
	FVector wishdir = m_wishdir.GetSafeNormal2D();
	float wishspeed = m_maxairspeed;

	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, m_acelerationairspeed * delta);

	// update speed
	UpdateVelocity(wishdir * addspeed);
}

void UPlayerMovement::DoFriction(float delta)
{
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
    return IsMovingOnGround() ? m_maxgroundspeed : m_maxairspeed;
}
