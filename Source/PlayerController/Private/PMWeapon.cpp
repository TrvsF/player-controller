#include "PMWeapon.h"

APMWeapon::APMWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);

	WeaponType		 = EWeaponType::RktLauncher;
	WeaponReloadType = EWeaponReloadType::OneByOne;
	WeaponShootType  = EWeaponShootType::Projectile;
	MaxMagSize = 4;
	ReloadTime = 0.8f;

	magsize = MaxMagSize;
	isreloading = false;
	remainingreloadtime = 0;
}

void APMWeapon::Tick(float DeltaTime)
{
	// reload
	if (isreloading)
	{
		remainingreloadtime -= DeltaTime;
		if (remainingreloadtime < 0)
		{
			OnScreenDebugger::DrawDebugMessage("RELOADING");

			if (WeaponReloadType == EWeaponReloadType::OneByOne)
			{
				magsize++;
				if (magsize == MaxMagSize)
				{
					isreloading = false;
					return;
				}
				remainingreloadtime = ReloadTime;
			}
			
			if (WeaponReloadType == EWeaponReloadType::Mag)
			{
				magsize = MaxMagSize;
				isreloading = false;
				return;
			}
		}
	}
}

void APMWeapon::Shoot(FVector origin, FRotator rotation)
{
	OnScreenDebugger::DrawDebugMessage(FString::FromInt(magsize), FColor::Blue, -1);
	if (magsize <= 0) { return; }

	if (WeaponShootType == EWeaponShootType::Projectile)
	{
		ShootProjectile(origin, rotation);
	}

	if (WeaponShootType == EWeaponShootType::HitScan)
	{
		ShootHitscan(origin, rotation);
	}

	// TODO : move me?
	magsize--;
	isreloading = false;

	// debug
	UWorld* World = GetWorld();
	if (World)
	{
		OnScreenDebugger::DrawDebugMessage("shooted");
		ARocket* SpawnedProjectile = World->SpawnActor<ARocket>(Projectile, origin, rotation);
		if (SpawnedProjectile)
		{
			OnScreenDebugger::DrawDebugMessage("fired");
		}
	}
}

void APMWeapon::Reload()
{
	if (magsize < MaxMagSize) 
	{ 
		isreloading = true;
		remainingreloadtime = ReloadTime;
	}
}

void APMWeapon::BeginPlay()
{
	Super::BeginPlay();
}