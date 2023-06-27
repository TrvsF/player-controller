#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"

#include "Components/StaticMeshComponent.h"

#include "PMChacter.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMChacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Character Mesh", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;

	UPROPERTY(Category = "Character Collision", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionBox;

	APMChacter(const FObjectInitializer& objectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
