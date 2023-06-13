#include "PlayerMovement.h"

UPlayerMovement::UPlayerMovement()
{
    m_maxwalkspeed = 600.f;
    m_maxairspeed  = 2000.f;
    m_acelerationspeed = 3000.f;
	m_friction = 12.f;
}

void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	m_tickcheck = false;

	// TODO : figure out exactly why this is needed -
	//		  its overriding *something* but im not sure what
	if (TickType == LEVELTICK_All)
	{
		// TickVelocity(DeltaTime);
	}

	FString velstr = FString::SanitizeFloat(Velocity.Size2D());
	OnScreenDebugger::DrawDebugMessage("vel: " + velstr, FColor::White, 10);
	OnScreenDebugger::DrawDebugMessage("onground", IsMovingOnGround(), 11);
}

void UPlayerMovement::TickVelocity(float delta)
{
	// TODO : shit hack
	if (m_tickcheck) 
	{
		OnScreenDebugger::DrawDebugMessage("tc", FColor::Red, -1);
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
	float wishvel = (Velocity + moveadd).Size2D();

	if (wishvel <= m_maxwalkspeed)
	{ Velocity += moveadd; }
}

void UPlayerMovement::UpdateVelocityGround(float delta)
{
	// wish
	FVector wishdir = GetWishDir().GetSafeNormal2D();
	float wishspeed = m_maxwalkspeed;

	// fpply friction
	float speed = Velocity.Size2D();
	if (speed != 0)
	{
		float drop = speed * m_friction * delta;
		Velocity *= FMath::Max(speed - drop, 0) / speed;
	}

	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, m_acelerationspeed * delta);

	// debug
	FString wishdirstr = wishdir.ToCompactString();
	OnScreenDebugger::DrawDebugMessage("wishdir: " + wishdirstr, FColor::Green, 20);

	FString wishvelstr = FString::SanitizeFloat(wishspeed);
	OnScreenDebugger::DrawDebugMessage("wishspd: " + wishvelstr, FColor::Green, 21);

	FString currentstr = FString::SanitizeFloat(currentspeed);
	OnScreenDebugger::DrawDebugMessage("currentspeed: " + currentstr, FColor::Green, 22);

	FString addstr = FString::SanitizeFloat(addspeed);
	OnScreenDebugger::DrawDebugMessage("add: " + addstr, FColor::Green, 23);

	FString onestr = FString::SanitizeFloat(wishspeed - currentspeed);
	OnScreenDebugger::DrawDebugMessage("1: " + onestr, FColor::Green, 24);

	FString twostr = FString::SanitizeFloat(m_acelerationspeed * delta);
	OnScreenDebugger::DrawDebugMessage("2: " + twostr, FColor::Green, 25);

	// update speed
	UpdateVelocity(wishdir * addspeed);

	// reset vars TODO : MOVEME
	m_fwdvalue = 0;
	m_rgtvalue = 0;
}

void UPlayerMovement::UpdateVelocityAir(float delta)
{
	FVector wishvel = GetWishDir();
	FVector wishdir = wishvel.GetSafeNormal2D();

	// get updated speed
	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(m_maxairspeed - currentspeed, 0, m_acelerationspeed * delta);

	FString wishdirstr = wishdir.ToCompactString();
	OnScreenDebugger::DrawDebugMessage("wishdir: " + wishdirstr, FColor::Red, 20);

	FString wishvelstr = wishvel.ToCompactString();
	OnScreenDebugger::DrawDebugMessage("wishvel: " + wishvelstr, FColor::Red, 21);

	FString currentstr = FString::SanitizeFloat(currentspeed);
	OnScreenDebugger::DrawDebugMessage("current: " + currentstr, FColor::Red, 22);

	FString addstr = FString::SanitizeFloat(addspeed);
	OnScreenDebugger::DrawDebugMessage("add: " + addstr, FColor::Red, 23);

	// update speed
	UpdateVelocity(wishdir * addspeed);

	// reset vars TODO : MOVEME
	m_fwdvalue = 0;
	m_rgtvalue = 0;
}

FVector UPlayerMovement::GetWishDir()
{
	// TODO : can be replaced with direct vector methods 
	FVector foward = GetOwner()->GetActorForwardVector();
	FVector right  = GetOwner()->GetActorRightVector();

	FVector wishdir;
	for (int i = 0; i < 3; i++)
	{ wishdir[i] = foward[i] * m_fwdvalue + right[i] * m_rgtvalue; }

	if (wishdir == FVector::Zero())
	{
		OnScreenDebugger::DrawDebugMessage("0", FColor::Blue, -1);
	}

	return wishdir;
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
    return IsMovingOnGround() ? m_maxwalkspeed : m_maxairspeed;
}
