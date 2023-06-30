#include "PMCharacter.h"

APMCharacter::APMCharacter(const FObjectInitializer& objectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

	// mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Character Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	// collsiion
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// movement
	MovementPointer = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

FVector APMCharacter::GetWishDir() const
{
	const auto& f = GetActorForwardVector() * MovementVector.X;
	const auto& r = GetActorRightVector() * MovementVector.Y;

	return f + r;
}

void APMCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// tick movement
	if (MovementPointer)
	{
		MovementPointer->UpdateWishDir(GetWishDir());
		MovementPointer->TickComponent(DeltaTime, LEVELTICK_All, NULL);
	}
}

void APMCharacter::AddVelocity(FVector Velocity)
{
	if (MovementPointer)
	{
		MovementPointer->Velocity += Velocity;
	}
}
