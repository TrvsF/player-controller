#include "PMCharacter.h"

APMCharacter::APMCharacter(const FObjectInitializer& objectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

	// mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	// collsiion
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// movement
	MovementPointer = Cast<UPlayerMovement>(ACharacter::GetMovementComponent());
}

void APMCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APMCharacter::AddVelocity(FVector Velocity)
{
}
