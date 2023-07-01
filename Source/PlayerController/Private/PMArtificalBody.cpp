#include "PMArtificalBody.h"

APMArtificalBody::APMArtificalBody(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer.SetDefaultSubobjectClass<UPlayerMovement>(ACharacter::CharacterMovementComponentName))
{

}

void APMArtificalBody::LookAtPos(FVector targetpos)
{
	// get new rotation
	FVector direction = (targetpos - GetActorLocation()).GetSafeNormal();
	FRotator newrotation = direction.Rotation();

	// set new rotation
	SetActorTransform({newrotation, GetActorLocation()});
}

void APMArtificalBody::BeginPlay()
{
	Super::BeginPlay();

	LookAtPos({ 0, 0, 0 });
	MovementVector.X = 1.0f;
}
