#include "PMHud.h"

APMHud::APMHud()
{
    XHairSize = 8;
    XHairThickness = 2;
    XHairGap = 2;

    drawcircle = false;
    drawcross = true;
    
    scaleoffset = 1.0f;
}

void APMHud::Tick(float DeltaTime)
{
    if (scaleoffset == 1.0f)
    { return; }

    const auto& shrinkfactor = 0.5f;
    

    if (scaleoffset < 1.0f)
    { scaleoffset = 1.0f; }
}

void APMHud::DrawCircle(FVector2D pos, float radius, FLinearColor colour)
{
    DrawCircle(pos.X, pos.Y, radius, colour);
}

void APMHud::DrawCircle(float posx, float posy, float radius, FLinearColor colour)
{
    // TODO : make sure can only call when canvas exists

    if (IsCanvasValid_WarnIfNot())
    {
        // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm 
        // bresenham implimentation
        
        // this is bad and flushes some game renderer thing but cannot find a way to render as a batch
        // TODO : find a way to do batch items
        float x = radius;
        float y = 0;
        float d = 1 - x;
        while (y <= x)
        {
            // TODO : make this smaller
            // TODO : make this faster
            FVector2D screenvec = { 1, 1 };
            Canvas->K2_DrawBox({ posx + x, posy + y }, screenvec);
            Canvas->K2_DrawBox({ posx + y, posy + x }, screenvec);
            Canvas->K2_DrawBox({ posx - x, posy - y }, screenvec);
            Canvas->K2_DrawBox({ posx - y, posy - x }, screenvec);
            Canvas->K2_DrawBox({ posx - y, posy + x }, screenvec);
            Canvas->K2_DrawBox({ posx - x, posy + y }, screenvec);
            Canvas->K2_DrawBox({ posx + y, posy - x }, screenvec);
            Canvas->K2_DrawBox({ posx + x, posy - y }, screenvec);
            
            y++;
            if (d < 0)
            {
                d += 2 * y + 1;
            }
            else
            {
                x--;
                d += 2 * (y - x) + 1;
            }
        }
    }
}

void APMHud::DrawXHair()
{
    FVector2D centrevec = { Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f };

    // x
    if (drawcross)
    {
        // h
        DrawLine(centrevec.X - XHairSize, centrevec.Y, centrevec.X - XHairGap * 2, centrevec.Y, FLinearColor::White, XHairThickness);
        DrawLine(centrevec.X + XHairGap * 2, centrevec.Y, centrevec.X + XHairSize, centrevec.Y, FLinearColor::White, XHairThickness);
        // v
        DrawLine(centrevec.X, centrevec.Y - XHairSize, centrevec.X, centrevec.Y - XHairGap * 2, FLinearColor::White, XHairThickness);
        DrawLine(centrevec.X, centrevec.Y + XHairGap * 2, centrevec.X, centrevec.Y + XHairSize, FLinearColor::White, XHairThickness);
    }
    

    // o
    if (drawcircle)
    {
        DrawCircle(centrevec, XHairGap * scaleoffset, FLinearColor::White);
    }
}

void APMHud::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas == nullptr)
    { return; }  

    DrawXHair();
}