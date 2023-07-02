#include "PMHud.h"

APMHud::APMHud()
{
    XHairSize = 5;
    XHairThickness = 1;
}

void APMHud::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr)
    { return; }

    FVector2D centrevec = { Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f };

    // draw cross
    DrawLine(centrevec.X - XHairSize, centrevec.Y, centrevec.X + XHairSize, centrevec.Y, FLinearColor::White, XHairThickness);
    DrawLine(centrevec.X, centrevec.Y - XHairSize, centrevec.X, centrevec.Y + XHairSize, FLinearColor::White, XHairThickness);
}