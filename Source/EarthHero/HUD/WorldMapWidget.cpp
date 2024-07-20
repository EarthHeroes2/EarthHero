#include "WorldMapWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

FVector2D UWorldMapWidget::ConvertWorldToMapPosition(const FVector2D& WorldPosition) const
{
    // Convert the world position to the map position
    float MapX = (WorldPosition.X + 100800.0f) / 201600.0f * 1000.0f - 500.0f;
    float MapY = (WorldPosition.Y + 100800.0f) / 201600.0f * 1000.0f - 500.0f;
    return FVector2D(MapX, MapY);
}

void UWorldMapWidget::SetPlayerPosition(int32 PlayerIndex, const FVector2D& Position)
{
    UImage* PlayerImages[4] = { Player1Image, Player2Image, Player3Image, Player4Image };

    if (PlayerIndex >= 0 && PlayerIndex < 4)
    {
        if (PlayerImages[PlayerIndex])
        {
            FVector2D MapPosition = ConvertWorldToMapPosition(Position);
            UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PlayerImages[PlayerIndex]);
            if (CanvasSlot)
            {
                CanvasSlot->SetPosition(MapPosition);
                PlayerImages[PlayerIndex]->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}

void UWorldMapWidget::SetPlayerRotation(int32 PlayerIndex, float Rotation)
{
    UImage* PlayerImages[4] = { Player1Image, Player2Image, Player3Image, Player4Image };

    if (PlayerIndex >= 0 && PlayerIndex < 4)
    {
        if (PlayerImages[PlayerIndex])
        {
            PlayerImages[PlayerIndex]->SetRenderTransformAngle(Rotation);
        }
    }
}

void UWorldMapWidget::UpdateForceField(int32 ForceFieldIndex, float CurrentTime, float GrowthDuration)
{
    if (ForceFieldIndex >= 0 && ForceFieldIndex < 4)
    {
        if (ForceFieldStartTimes[ForceFieldIndex] == 0.0f)
        {
            ForceFieldStartTimes[ForceFieldIndex] = CurrentTime;
            ForceFieldTargetSizes[ForceFieldIndex] = 500.0f;
            ForceFieldGrowthRates[ForceFieldIndex] = 500.0f / GrowthDuration;
        }

        float ElapsedTime = CurrentTime - ForceFieldStartTimes[ForceFieldIndex];
        UImage* ForceFieldImages[4] = { ForceField1Image, ForceField2Image, ForceField3Image, ForceField4Image };
        if (ForceFieldImages[ForceFieldIndex] && ElapsedTime <= GrowthDuration)
        {
            ForceFieldCurrentSizes[ForceFieldIndex] = FMath::Min(ForceFieldGrowthRates[ForceFieldIndex] * ElapsedTime, ForceFieldTargetSizes[ForceFieldIndex]);
            UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ForceFieldImages[ForceFieldIndex]);
            if (CanvasSlot)
            {
                CanvasSlot->SetSize(FVector2D(ForceFieldCurrentSizes[ForceFieldIndex], ForceFieldCurrentSizes[ForceFieldIndex]));
            }
        }
    }
}

void UWorldMapWidget::SetForceFieldAlignment(int32 ForceFieldIndex, const FVector2D& Alignment)
{
    if (ForceFieldIndex >= 0 && ForceFieldIndex < 4)
    {
        ForceFieldAlignments[ForceFieldIndex] = Alignment;

        UImage* ForceFieldImages[4] = { ForceField1Image, ForceField2Image, ForceField3Image, ForceField4Image };
        if (ForceFieldImages[ForceFieldIndex])
        {
            UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ForceFieldImages[ForceFieldIndex]);
            if (CanvasSlot)
            {
                CanvasSlot->SetAlignment(Alignment);
            }
        }
    }
}
