#include "playercharacter.h"

Aplayercharacter::Aplayercharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	BaseTurnRate	 = 45.0f;
	BaseLookUpAtRate = 45.0f;
}

void Aplayercharacter::Jump()
{
	Super::Jump();
	
	FVector Facing = GetActorForwardVector();
	FVector JumpBoostedVel = GetMovementComponent()->Velocity + Facing * 5000.0f;
	GetMovementComponent()->Velocity = JumpBoostedVel;

	DrawDebugMessage("jump", FColor::Green, 1);
}

void Aplayercharacter::StopJumping()
{
	Super::StopJumping();
}

void Aplayercharacter::DrawDebugMessage(char* message, FColor colour, int id)
{
	const auto& fstring = FString(message);
	DrawDebugMessage(fstring, colour, id);
}

void Aplayercharacter::DrawDebugMessage(FString message, FColor colour, int id)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(id, 4.0f, colour, message);
	}
}

// Called when the game starts or when spawned
void Aplayercharacter::BeginPlay()
{
	Super::BeginPlay();

	// use strcat
	DrawDebugMessage("spawned player", FColor::Green, -1);
}

void Aplayercharacter::MoveForward(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Aplayercharacter::StrafeRight(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void Aplayercharacter::TurnAtRate(float Value)
{
	
}

void Aplayercharacter::LookUpAtRate(float Value)
{
}

// Called every frame
void Aplayercharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const FVector Velocity = GetMovementComponent()->Velocity;
	const FVector Position = GetActorLocation();
	DrawDebugMessage(Velocity.ToCompactString(), FColor::Black, 2);
	DrawDebugMessage(Position.ToCompactString(), FColor::Black, 3);
}

// Called to bind functionality to input
void Aplayercharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &Aplayercharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &Aplayercharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Aplayercharacter::MoveForward);
	PlayerInputComponent->BindAxis("StrafeRight", this, &Aplayercharacter::StrafeRight);

	PlayerInputComponent->BindAxis("Yaw",   this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APawn::AddControllerPitchInput);
}

