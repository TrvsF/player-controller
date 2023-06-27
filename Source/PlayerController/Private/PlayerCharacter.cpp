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

	// movement
	m_movementptr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

FVector APlayerCharacter::GetWishDir() const
{
	const auto& f = GetActorForwardVector() * m_movementvector.X;
	const auto& r = GetActorRightVector()   * m_movementvector.Y;

	return f + r;
}

void APlayerCharacter::Shoot()
{
	UWorld* World = GetWorld();
	if (World)
	{
		float distancefromplayer = 150.f;
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * distancefromplayer;
		FRotator SpawnRotation = FirstPersonCameraComp->GetComponentRotation();

		ARocket* SpawnedProjectile = World->SpawnActor<ARocket>(Rocket, SpawnLocation, SpawnRotation);

		if (SpawnedProjectile)
		{
			OnScreenDebugger::DrawDebugMessage("fired", FColor::Green, -1);
		}
	}
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
	if (Controller)
	{
		m_movementvector.X = Value;
	}
}

void APlayerCharacter::Strafe(float Value)
{
	if (Controller)
	{
		m_movementvector.Y = Value;
	}
}

// called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// tick movement
	if (m_movementptr)
	{
		m_movementptr->UpdateWishDir(GetWishDir());
		m_movementptr->TickComponent(DeltaTime, LEVELTICK_All, NULL);
	}

	// debug
	const auto& PositionStr = GetActorLocation().ToCompactString();
	OnScreenDebugger::DrawDebugMessage("pos: " + PositionStr, FColor::White, 0);
	const auto& WishStr = GetWishDir().ToCompactString();
	OnScreenDebugger::DrawDebugMessage("wishdir: " + WishStr, FColor::White, 1);
}

// called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// TODO : replace with the new UE5 shit
	PlayerInputComponent->BindAction("Jump",  IE_Pressed,  this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed,  this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::Move);
	PlayerInputComponent->BindAxis("StrafeRight", this, &APlayerCharacter::Strafe);
	PlayerInputComponent->BindAxis("Yaw",   this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APlayerCharacter::AddControllerPitchInput);
}