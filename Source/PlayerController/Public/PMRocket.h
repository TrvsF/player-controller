#pragma once

#include "CoreMinimal.h"

#include "OnScreenDebugger.h"
#include "PMCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PMRocket.generated.h"

UCLASS()
class PLAYERCONTROLLER_API ARocket : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Category = "Rocket Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;
	
	UPROPERTY(Category = "Rocket Movement", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovementComp;

	void Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(Category = "Rocket", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float ExplosionRadius;

	UPROPERTY(Category = "Rocket", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RocketSpeed;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	ARocket();
};
