#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EWeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Pistol      UMETA(DisplayName = "Pistol"),
    RktLauncher UMETA(DisplayName = "Rocket Launcher"),
    Smg         UMETA(DisplayName = "PPSH"),
};

UENUM(BlueprintType)
enum class EWeaponReloadType : uint8
{
    Mag      UMETA(DisplayName = "Mag"),
    OneByOne UMETA(DisplayName = "One By One"),
};

UENUM(BlueprintType)
enum class EWeaponShootType : uint8
{
    Projectile UMETA(DisplayName = "Projectile"),
    HitScan    UMETA(DisplayName = "Hitscan"),
};