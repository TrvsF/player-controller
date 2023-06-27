#include "PMPlayer.h"

APMPlayer::APMPlayer(const FObjectInitializer& objectInitializer)
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
}

FVector APMPlayer::GetWishDir() const
{
	const auto& f = GetActorForwardVector() * m_movementvector.X;
	const auto& r = GetActorRightVector()   * m_movementvector.Y;

	return f + r;
}

void APMPlayer::Shoot()
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

void APMPlayer::Jump()
{
	MovementPointer->DoJump(false);
}

void APMPlayer::AddControllerYawInput(float Value)
{
	Value *= YawSensMultiplyer;
	Super::AddControllerYawInput(Value);
}

void APMPlayer::AddControllerPitchInput(float Value)
{
	Value *= PitchSensMultiplyer;
	Super::AddControllerPitchInput(Value);
}

// called when the game starts or when spawned
void APMPlayer::BeginPlay()
{
	Super::BeginPlay();
	OnScreenDebugger::DrawDebugMessage("spawned player", FColor::Green, -1);
}

void APMPlayer::Move(float Value)
{
	if (Controller)
	{
		m_movementvector.X = Value;
	}
}

void APMPlayer::Strafe(float Value)
{
	if (Controller)
	{
		m_movementvector.Y = Value;
	}
}

// called every frame
void APMPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// tick movement
	if (MovementPointer)
	{
		MovementPointer->UpdateWishDir(GetWishDir());
		MovementPointer->TickComponent(DeltaTime, LEVELTICK_All, NULL);
	}

	// debug
	const auto& PositionStr = GetActorLocation().ToCompactString();
	OnScreenDebugger::DrawDebugMessage("pos: " + PositionStr, FColor::White, 0);
	const auto& WishStr = GetWishDir().ToCompactString();
	OnScreenDebugger::DrawDebugMessage("wishdir: " + WishStr, FColor::White, 1);
}

// called to bind functionality to input
void APMPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// TODO : replace with the new UE5 shit
	PlayerInputComponent->BindAction("Jump",  IE_Pressed,  this, &APMPlayer::Jump);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed,  this, &APMPlayer::Shoot);
	PlayerInputComponent->BindAxis("MoveForward", this, &APMPlayer::Move);
	PlayerInputComponent->BindAxis("StrafeRight", this, &APMPlayer::Strafe);
	PlayerInputComponent->BindAxis("Yaw",   this, &APMPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APMPlayer::AddControllerPitchInput);
}

void APMPlayer::AddVelocity(FVector Velocity)
{
	if (MovementPointer)
	{
		MovementPointer->Velocity += Velocity;
	}
}
