#pragma once

#include "CoreMinimal.h"

#include "EWeaponType.h"

#include "GameFramework/Actor.h"

#include "PMWeapon.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMWeapon : public AActor
{
	GENERATED_BODY()

public:
	APMWeapon();
	
private:
	EWeaponType WeaponType;

protected:
	virtual void BeginPlay() override;
};
