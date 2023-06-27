#include "PMRocket.h"

ARocket::ARocket()
{
	// vars
	ExplosionRadius = 400.0f;
	RocketSpeed     = 3600.0f;
	InitialLifeSpan = 5.0f;

	PrimaryActorTick.bCanEverTick = true;

	// rocket mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	// movement 
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Rocket Movement"));
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->InitialSpeed = RocketSpeed;
	ProjectileMovementComp->MaxSpeed = RocketSpeed;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->ProjectileGravityScale = 0;
}

void ARocket::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// <<<assuming is exploding>>>
	// get hit result of explosion sphere
	TArray<FHitResult> hitresults;
	FCollisionShape collisionsphere;
	collisionsphere.SetSphere(ExplosionRadius);

	// get hit results from sphere
	FVector hitrocketlocation = GetActorLocation();
	if (GetWorld()->SweepMultiByChannel(hitresults, hitrocketlocation, hitrocketlocation, FQuat::Identity, ECC_Pawn, collisionsphere))
	{
		for (const FHitResult& hitresult : hitresults)
		{
			AActor* hitactor = hitresult.GetActor();
			if (hitactor != nullptr)
			{
				OnScreenDebugger::DrawDebugMessage("rocket hit something", FColor::Red, 777);
				// if actor is a character
				if (APMCharacter* hitcharacter = Cast<APMCharacter>(hitactor))
				{
					OnScreenDebugger::DrawDebugMessage("rocket hit player", FColor::Red, 778);

					FVector hitcharacterlocation = hitcharacter->GetActorLocation();	
					FHitResult rockethitresult;

					FCollisionObjectQueryParams objparams = FCollisionObjectQueryParams(ECC_WorldStatic);
					objparams.AddObjectTypesToQuery(ECC_WorldStatic);

					// if actor is not blocked by a wall
					if (!GetWorld()->LineTraceSingleByObjectType(rockethitresult, hitrocketlocation, hitcharacterlocation, objparams))
					{
						OnScreenDebugger::DrawDebugMessage("rocket can see player", FColor::Green, 779);
						
						// calc kockback
						FVector knockback = { 0, 0, 10000.f };
						hitcharacter->AddVeloicty(knockback);
					}
				}
			}
		}
	}

	// debug
	DrawDebugSphere(GetWorld(), hitrocketlocation, ExplosionRadius, 16, FColor::Blue, false, 2.0f);
}

void ARocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARocket::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	/*
		below doesnt fire for some reason so the event is handled within the blueprint
		the EndPlay method handles the explosion of the rocket

		TODO : clean up & figure out why this doesnt work
	*/
	MeshComp->OnComponentHit.AddDynamic(this, &ARocket::Explode);

	// debug
	OnScreenDebugger::DrawDebugMessage("spawned rocket from rocket", FColor::Green, -1);
}