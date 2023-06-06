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

	MovementPtr = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

void APlayerCharacter::Jump()
{
	Super::Jump();

	DrawDebugMessage("jump", FColor::Green, 1);
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();
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

	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Enable collision for the box

		// Set collision response for the box
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		// Add more collision channels and their responses if needed
	}

	// use strcat
	DrawDebugMessage("spawned player", FColor::Green, -1);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		GetMovementComponent()->AddInputVector(GetActorForwardVector() * Value);
	}
}

void APlayerCharacter::StrafeRight(float Value)
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
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("StrafeRight", this, &APlayerCharacter::StrafeRight);

	PlayerInputComponent->BindAxis("Yaw",   this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APlayerCharacter::AddControllerPitchInput);
}

