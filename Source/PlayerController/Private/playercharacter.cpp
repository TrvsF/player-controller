#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UPlayerMovement>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	// camera
	FirstPersonCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComp->SetupAttachment(RootComponent);
	FirstPersonCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FirstPersonCameraComp->bUsePawnControlRotation = true;

	// collsiion
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MovementPtr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

void APlayerCharacter::Jump()
{
	MovementPtr->DoJump(false);
}

void APlayerCharacter::AddControllerYawInput(float Value)
{
	Value *= 0.38f;
	Super::AddControllerYawInput(Value);
}

void APlayerCharacter::AddControllerPitchInput(float Value)
{
	Value *= 0.25f;
	Super::AddControllerPitchInput(Value);
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
		GetMovementComponent()->AddInputVector(GetActorForwardVector() * Value);
	}
}

void APlayerCharacter::Strafe(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		GetMovementComponent()->AddInputVector(GetActorRightVector() * Value);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const auto& VelocityStr = GetMovementComponent()->Velocity.ToCompactString();
	const auto& PositionStr = GetActorLocation().ToCompactString();
	DrawDebugMessage("vel:" + VelocityStr, FColor::White, 2);
	DrawDebugMessage("pos:" + PositionStr, FColor::White, 3);
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