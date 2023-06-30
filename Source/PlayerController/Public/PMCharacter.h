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
	APMCharacter(const FObjectInitializer& objectInitializer);

	FVector GetWishDir() const;
	void AddVelocity(FVector Velocity);

	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(Category = "Character Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	UPROPERTY(Category = "Character Collision", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;

protected:
	FVector MovementVector;
	UPlayerMovement* MovementPointer;

	virtual void BeginPlay() override;
};
