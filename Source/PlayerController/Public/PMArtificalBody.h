#pragma once

#include "CoreMinimal.h"
#include "PMCharacter.h"
#include "PMArtificalBody.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMArtificalBody : public APMCharacter
{
	GENERATED_BODY()

public:
	// APMArtificalBody(const FObjectInitializer& objectInitializer);

	// FVector GetWishDir() const;

	// virtual void Tick(float DeltaTime) override;
	// virtual void AddVelocity(FVector Velocity) override;

private:
	FVector MovementVector;

protected:
	// virtual void BeginPlay() override;

};
