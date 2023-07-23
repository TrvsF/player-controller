#pragma once

#include "CoreMinimal.h"

#include "OnScreenDebugger.h"
#include "PMRocket.h"
#include "EWeaponType.h"

#include "GameFramework/Actor.h"

#include "PMWeapon.generated.h"

UCLASS()
class TowerDefence_API APMWeapon : public AActor
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

	void Shoot(FVector origin, FRotator rotation);
	virtual void ShootHitscan(FVector origin, FRotator rotation) {}
	virtual void ShootProjectile(FVector origin, FRotator rotation) {}

	void Reload();

private:
	// meta-data on the weapon
	EWeaponType WeaponType;
	EWeaponReloadType WeaponReloadType;
	EWeaponShootType WeaponShootType;
	int magsize;

	// reloading
	bool isreloading;
	float remainingreloadtime;

	inline bool CanShoot()
	{ return magsize > 0; }

protected:
	virtual void BeginPlay() override;

};
