#pragma once

#include "CoreMinimal.h"

#include "PlayerMovement.h"

#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PMCharacter.generated.h"

class UPlayerMovement;

UCLASS()
class PLAYERCONTROLLER_API APMCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Character Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	UPROPERTY(Category = "Character Collision", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;

	APMCharacter(const FObjectInitializer& objectInitializer);

	UPlayerMovement* MovementPointer;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	inline void AddVeloicty(FVector Velocity)
	{
		MovementPointer->Velocity += Velocity;
	}
};
