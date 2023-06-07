#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UPlayerMovement>(ACharacter::CharacterMovementComponentName))
{
 	// tick every frame
	PrimaryActorTick.bCanEverTick = true;

	// camera
	FirstPersonCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComp->SetupAttachment(RootComponent);
	FirstPersonCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FirstPersonCameraComp->bUsePawnControlRotation = true;
	YawSensMultiplyer	= 0.4f;
	PitchSensMultiplyer = 0.3f;

	// player mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	// collsiion
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// movement
	MovementPtr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

void APlayerCharacter::Jump()
{
	MovementPtr->DoJump(false);
}

void APlayerCharacter::AddControllerYawInput(float Value)
{
	Value *= YawSensMultiplyer;
	Super::AddControllerYawInput(Value);
}

void APlayerCharacter::AddControllerPitchInput(float Value)
{
	Value *= PitchSensMultiplyer;
	Super::AddControllerPitchInput(Value);
}

void APlayerCharacter::DrawDebugMessage(char* message, bool boolean, int id)
{
	const auto& colour  = boolean ? FColor::Green : FColor::Red;
	const auto& fstring = FString(message);

	DrawDebugMessage(fstring, colour, id);
}

void APlayerCharacter::DrawDebugMessage(char* message, FColor colour, int id)
{
	const auto& fstring = FString(message);

	DrawDebugMessage(fstring, colour, id);
}

void APlayerCharacter::DrawDebugMessage(FString message, FColor colour, int id)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(id, 4.0f, colour, message);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// use strcat
	DrawDebugMessage("spawned player", FColor::Green, -1);
}

void APlayerCharacter::Move(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::Strafe(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const auto& PositionStr = GetActorLocation().ToCompactString();
	const auto& VelocityStr = FString::SanitizeFloat(GetMovementComponent()->Velocity.Size());
	const auto& AccelStr    = FString::SanitizeFloat(MovementPtr->GetCurrentAcceleration().Size());

	DrawDebugMessage("pos:  " + PositionStr, FColor::White, 2);
	DrawDebugMessage("vel:  " + VelocityStr, FColor::White, 3);
	DrawDebugMessage("accel:" + VelocityStr, FColor::White, 4);

	const auto& IsOnGround = MovementPtr->IsMovingOnGround();
	DrawDebugMessage("onground", IsOnGround, 5);

	
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &APlayerCharacter::Jump);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::Move);
	PlayerInputComponent->BindAxis("StrafeRight", this, &APlayerCharacter::Strafe);

	PlayerInputComponent->BindAxis("Yaw",   this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APlayerCharacter::AddControllerPitchInput);
}

void APlayerCharacter::OnMovementModeChanged(EMovementMode prevMode, uint8 prevCustomMode)
{
	if (!bPressedJump) {
		ResetJumpState();
	}

	if (MovementPtr->IsFalling() && bProxyIsJumpForceApplied) {
		ProxyJumpForceStartedTime = GetWorld()->GetTimeSeconds();
	}
	else {
		JumpCurrentCount = 0;
		JumpKeyHoldTime = 0.f;
		JumpForceTimeRemaining = 0.f;
		bWasJumping = false;
	}

	K2_OnMovementModeChanged(prevMode, MovementPtr->MovementMode, prevCustomMode, MovementPtr->CustomMovementMode);
	MovementModeChangedDelegate.Broadcast(this, prevMode, prevCustomMode);
}
