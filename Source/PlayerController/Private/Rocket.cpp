#include "Rocket.h"

ARocket::ARocket()
{
	// rocket mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetWorldScale3D({ 1.f, 0.3f, 0.3f });

	// TODO : cringe (should probly just use blueprints)
	UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere")); 
	MeshComp->SetStaticMesh(SphereMesh);

	// collsiion <<box>>
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Rocket Collision"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetWorldScale3D({ 1.3f, 0.3f, 0.3f });

	// movement 
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Rocket Movement"));
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->InitialSpeed = 3200.0f;
	ProjectileMovementComp->MaxSpeed = 4000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
}

void ARocket::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	OnScreenDebugger::DrawDebugMessage("boom", FColor::Red, -1);

	Destroy();
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	// inits
	ProjectileMovementComp->Activate();
	CollisionBox->OnComponentHit.AddDynamic(this, &ARocket::Explode);

	// debug
	OnScreenDebugger::DrawDebugMessage("spawned rocket", FColor::Green, -1);
}

void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

