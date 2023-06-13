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
	m_movementptr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

void APlayerCharacter::Jump()
{
	m_movementptr->DoJump(false);
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

// called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnScreenDebugger::DrawDebugMessage("spawned player", FColor::Green, -1);
}

void APlayerCharacter::Move(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		m_movementptr->AddFowardInput(Value);
	}
}

void APlayerCharacter::Strafe(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		m_movementptr->AddStrafeInput(Value);
	}
}

// called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_movementptr)
	{
		m_movementptr->TickComponent(DeltaTime, LEVELTICK_All, NULL);
	}

	const auto& PositionStr = GetActorLocation().ToCompactString();
	OnScreenDebugger::DrawDebugMessage("pos: " + PositionStr, FColor::White, 0);
}

// called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &APlayerCharacter::Jump);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::Move);
	PlayerInputComponent->BindAxis("StrafeRight", this, &APlayerCharacter::Strafe);

	PlayerInputComponent->BindAxis("Yaw",   this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APlayerCharacter::AddControllerPitchInput);
}

// TODO : remove me?
void APlayerCharacter::OnMovementModeChanged(EMovementMode prevMode, uint8 prevCustomMode)
{
	if (!bPressedJump)
	{
		ResetJumpState();
	}

	if (m_movementptr->IsFalling() && bProxyIsJumpForceApplied) 
	{
		ProxyJumpForceStartedTime = GetWorld()->GetTimeSeconds();
	}
	else 
	{
		JumpCurrentCount = 0;
		JumpKeyHoldTime = 0.f;
		JumpForceTimeRemaining = 0.f;
		bWasJumping = false;
	}

	K2_OnMovementModeChanged(prevMode, m_movementptr->MovementMode, prevCustomMode, m_movementptr->CustomMovementMode);
	MovementModeChangedDelegate.Broadcast(this, prevMode, prevCustomMode);
}
