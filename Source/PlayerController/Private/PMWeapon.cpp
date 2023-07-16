#include "PMWeapon.h"

APMWeapon::APMWeapon()
{
	WeaponType = EWeaponType::RktLauncher;
	MaxMagSize = 4;
	ReloadTime = 0.8f;

	magsize = MaxMagSize;
}

void APMWeapon::Tick(float DeltaTime)
{
	// NOT TICKING
}

void APMWeapon::BeginPlay()
{
	Super::BeginPlay();
}