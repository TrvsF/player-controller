#include "PMHud.h"

void APMHud::DrawHUD()
{
    Super::DrawHUD();

    // Calculate the center of the screen
    FVector2D ScreenCenter(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

    // Set the crosshair size and thickness
    int32 CrosshairSize = 20;
    int32 CrosshairThickness = 2;

    // Draw horizontal line of the crosshair
    DrawLine(ScreenCenter.X - CrosshairSize, ScreenCenter.Y, ScreenCenter.X + CrosshairSize, ScreenCenter.Y, FLinearColor::White, CrosshairThickness);

    // Draw vertical line of the crosshair
    DrawLine(ScreenCenter.X, ScreenCenter.Y - CrosshairSize, ScreenCenter.X, ScreenCenter.Y + CrosshairSize, FLinearColor::White, CrosshairThickness);
}