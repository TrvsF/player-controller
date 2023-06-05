#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	FirstPersonCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComp->SetupAttachment(RootComponent);
	FirstPersonCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	FirstPersonCameraComp->bUsePawnControlRotation = true;
}

void APlayerCharacter::Jump()
{
	Super::Jump();
	
	FVector Facing = GetActorForwardVector();
	FVector JumpBoostedVel = GetMovementComponent()->Velocity + Facing * 5000.0f;
	GetMovementComponent()->Velocity = JumpBoostedVel;

	DrawDebugMessage("jump", FColor::Green, 1);
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();
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

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::StrafeRight(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const auto& VelocityStr = GetMovementComponent()->Velocity.ToCompactString();
	const auto& PositionStr = GetActorLocation().ToCompactString();
	DrawDebugMessage("vel:" + VelocityStr, FColor::Black, 2);
	DrawDebugMessage("pos:" + PositionStr, FColor::Black, 3);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("StrafeRight", this, &APlayerCharacter::StrafeRight);

	PlayerInputComponent->BindAxis("Yaw",   this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APawn::AddControllerPitchInput);
}

