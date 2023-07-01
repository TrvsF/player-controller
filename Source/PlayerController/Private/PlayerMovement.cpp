#include "PlayerMovement.h"

UPlayerMovement::UPlayerMovement()
{
	// init vars
	MaxGroundSpeed = 550.f;
	MaxAirSpeed    = 2400.f;
	GroundAccelerationSpeed = 3400.f;
	AirAccelerationSpeed    = 1800.f;
	BaseGroundFriction = 1500.f;
	JumpSpeed = 400.f;
	// TODO : remove base stuff?
	GravityScale = 0.6f;
}

void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	m_tickcheck = false;
	/*
		note: 
		forcing the movement to match the character tick probably isnt a good idea
		however this is the way i got working for smooth, relyable inputs. 

		TODO :
		more work needs to go into looking at how events work, as i think they drive 
		the movement componant
	*/
}

void UPlayerMovement::TickVelocity(float delta)
{
	// TODO : shit hack
	if (m_tickcheck) 
	{ return; }
	m_tickcheck = true;

	// tick velocity
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
	// debug
	const auto& addstr = FString::SanitizeFloat(moveadd.Size2D());

	// onground differences
	bool onground = IsMovingOnGround();
	float maxspeed = onground ? MaxGroundSpeed : MaxAirSpeed;
	
	// speed we want
	float wishspeed = (Velocity + moveadd).Size2D();

	// speed we get
	if (wishspeed <= maxspeed)
	{ 
		Velocity += moveadd;
	}
	else 
	{ 
		// clamps the vel to the max speed
		float maxadd = maxspeed - Velocity.Size2D();
		Velocity += moveadd.GetClampedToMaxSize2D(maxadd);
	}
}

void UPlayerMovement::UpdateVelocityGround(float delta)
{
	// wish
	FVector wishdir = m_wishdir.GetSafeNormal2D();
	float wishspeed = MaxGroundSpeed;
	
	// friction
	DoFriction(delta);

	// work out speed to add
	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, GroundAccelerationSpeed * delta);

	// update speed
	UpdateVelocity(wishdir * addspeed);
}

void UPlayerMovement::UpdateVelocityAir(float delta)
{
	// wish
	FVector wishdir = m_wishdir.GetSafeNormal2D();
	float wishspeed = 20.f; 

	// work out speed to add
	float currentspeed = FVector::DotProduct(Velocity, wishdir);
	float addspeed = FMath::Clamp(wishspeed - currentspeed, 0, AirAccelerationSpeed * delta);

	// update speed
	UpdateVelocity(wishdir * addspeed);
}

void UPlayerMovement::DoFriction(float delta)
{
	float speed = Velocity.Size2D();
	if (speed != 0)
	{
		// clamp if speed is less than 1
		if (speed < 1.f)
		{
			Velocity = { 0, 0, Velocity.Z };
			return;
		}
		// else decel based on current speed
		float newspeed = FMath::Max(0, speed - BaseGroundFriction * delta);
		newspeed /= speed;
		// update vel
		Velocity *= newspeed;
	}
}

void UPlayerMovement::CalcVelocity(float delta, float friction, bool bFluid, float brakingDeceleration)
{
	TickVelocity(delta);
}

bool UPlayerMovement::DoJump(bool bClientSimulation)
{
	if (CharacterOwner && CharacterOwner->CanJump())
	{
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			Velocity.Z = FMath::Max<FVector::FReal>(Velocity.Z, JumpSpeed);
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	return false;
}

float UPlayerMovement::GetMaxSpeed() const
{
    return IsMovingOnGround() ? MaxGroundSpeed : MaxAirSpeed;
}
