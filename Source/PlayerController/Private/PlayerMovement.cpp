#include "PlayerMovement.h"

UPlayerMovement::UPlayerMovement()
{
	// max speed
    m_maxgroundspeed = 550.f;
    m_maxairspeed    = 2400.f;
	// accel
    m_acelerationgroundspeed = 3000.f;
    m_acelerationairspeed	 = 1800.f;
	// friction
	m_friction = 1500.f;
	// jump
	m_jumpspeed = 450.f;

	GravityScale = 0.6f;
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

	// onground differences
	bool onground = IsMovingOnGround();
	float maxspeed = onground ? m_maxgroundspeed : m_maxairspeed;
	
	// vel we want
	float wishvel = (Velocity + moveadd).Size2D();

	// vel we get
	if (wishvel <= maxspeed)
	{ 
		Velocity += moveadd;
	}
	// debug
	else 
	{ 
		Velocity += moveadd.GetClampedToMaxSize2D(maxspeed - Velocity.Size2D());
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
	float wishspeed = 15.f;

	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, m_acelerationairspeed * delta);

	/*
	const auto& dspeedstr = FString::SanitizeFloat(m_acelerationairspeed * delta);
	OnScreenDebugger::DrawDebugMessage(dspeedstr, FColor::Green, 110);
	*/

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
	if (CharacterOwner && CharacterOwner->CanJump())
	{
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			Velocity.Z = FMath::Max<FVector::FReal>(Velocity.Z, m_jumpspeed);
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	return false;
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
