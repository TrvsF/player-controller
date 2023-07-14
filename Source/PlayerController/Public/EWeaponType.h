#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EWeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Pistol UMETA(DisplayName = "PPSH"),
    RktLauncher UMETA(DisplayName = "Rocket Launcher"),
    Smg UMETA(DisplayName = "PPSH"),
};
