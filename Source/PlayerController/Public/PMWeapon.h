#pragma once

#include "CoreMinimal.h"

#include "OnScreenDebugger.h"
#include "PMRocket.h"
#include "EWeaponType.h"

#include "GameFramework/Actor.h"

#include "PMWeapon.generated.h"

UCLASS()
class PLAYERCONTROLLER_API APMWeapon : public AActor
{
	GENERATED_BODY()

public:
	APMWeapon();

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		int MaxMagSize;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0"))
		float ReloadTime;

	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ARocket> Projectile;

	virtual void Tick(float DeltaTime) override;

	// virtual void Shoot(FVector location, FRotator rotation) {}
	// debug
	void Shoot(FVector location, FRotator rotation)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			OnScreenDebugger::DrawDebugMessage("shooted");
			ARocket* SpawnedProjectile = World->SpawnActor<ARocket>(Projectile, location, rotation);
			if (SpawnedProjectile)
			{
				OnScreenDebugger::DrawDebugMessage("fired");
			}
		}
	}

private:
	EWeaponType WeaponType;
	int magsize;

	inline bool CanShoot()
	{ return magsize > 0; }

protected:
	virtual void BeginPlay() override;

};
