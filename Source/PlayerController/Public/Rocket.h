#pragma once

#include "CoreMinimal.h"

#include "OnScreenDebugger.h"

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Rocket.generated.h"

UCLASS()
class PLAYERCONTROLLER_API ARocket : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Category = "Rocket Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	/*
	UPROPERTY(Category = "Rocket Collision", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;
	*/
	
	UPROPERTY(Category = "Rocket Movement", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovementComp;
	 
	void Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	ARocket();
};
