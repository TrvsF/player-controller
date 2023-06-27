#include "PMChacter.h"

APMChacter::APMChacter(const FObjectInitializer& objectInitializer)
{
 	PrimaryActorTick.bCanEverTick = true;

	// mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	// collsiion
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APMChacter::BeginPlay()
{
	Super::BeginPlay();
}

void APMChacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}