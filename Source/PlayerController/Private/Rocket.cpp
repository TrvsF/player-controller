#include "Rocket.h"

void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnScreenDebugger::DrawDebugMessage(GetActorLocation().ToCompactString(), FColor::Green, 781);
}

void ARocket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// assuming is exploding
	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(ExplosionRadius);

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation; 
	bool bTraceComplex = false; 

	if (GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC_Pawn, CollisionShape))
	{
		for (const FHitResult& HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor != nullptr)
			{
				// TODO : make only care about player/ai
				OnScreenDebugger::DrawDebugMessage("FOUND EXPLOSION TARGET", FColor::Green, -1);
			}
		}
	}

	// Visualize the collision shape for debugging
	DrawDebugSphere(GetWorld(), StartLocation, ExplosionRadius, 16, FColor::Blue, false, 2.0f);
}

ARocket::ARocket()
{
	// vars
	ExplosionRadius = 1200.0f;

	// rocket mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	// movement 
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Rocket Movement"));
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->InitialSpeed = 3200.0f;
	ProjectileMovementComp->MaxSpeed = 4000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0;
}

void ARocket::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	InitialLifeSpan = 5.0f;
	PrimaryActorTick.bCanEverTick = true;

	/*
		below doesnt fire for some reason so the event is handled within the blueprint
		the EndPlay method handles the explosion of the rocket

		TODO : clean up & figure out why this doesnt work
	*/
	MeshComp->OnComponentHit.AddDynamic(this, &ARocket::Explode);

	// debug
	OnScreenDebugger::DrawDebugMessage("spawned rocket from rocket", FColor::Green, -1);
}