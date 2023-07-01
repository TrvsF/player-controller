#pragma once

#include "CoreMinimal.h"
#include "PMCharacter.h"
#include "PMArtificalBody.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMArtificalBody : public APMCharacter
{
	GENERATED_BODY()

public:
	APMArtificalBody(const FObjectInitializer& objectInitializer);

	// virtual void Tick(float DeltaTime) override;

	void LookAtPos(FVector targetpos);
private:


protected:
	virtual void BeginPlay() override;

};
