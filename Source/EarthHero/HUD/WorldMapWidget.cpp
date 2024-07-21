#include "WorldMapWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UWorldMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ForceFieldCurrentSizes.Init(0.0f, 4);
    ForceFieldTargetSizes.Init(0.0f, 4);
    ForceFieldGrowthRates.Init(0.0f, 4);
    ForceFieldStartTimes.Init(0.0f, 4);
    ForceFieldAlignments.Init(FVector2D(0.5f, 0.5f), 4);
}

FVector2D UWorldMapWidget::ConvertWorldToMapPosition(const FVector2D& WorldPosition) const
{
    // Map dimensions
    float MapWidth = 689.5f;
    float MapHeight = 706.5f;
    
    float MapX = (WorldPosition.Y + 100800.0f) / 201600.0f * MapWidth - (MapWidth / 2.0f);
    float MapY = (MapHeight / 2.0f) - (WorldPosition.X + 100800.0f) / 201600.0f * MapHeight;

    return FVector2D(MapX, MapY);
}

void UWorldMapWidget::SetPlayerPosition(int32 PlayerIndex, const FVector2D& Position)
{
    UImage* PlayerImages[4] = { Player1Image, Player2Image, Player3Image, Player4Image };

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

void UWorldMapWidget::SetPlayerRotation(int32 PlayerIndex, float Rotation)
{
    UImage* PlayerImages[4] = { Player1Image, Player2Image, Player3Image, Player4Image };
    
    if (PlayerImages[PlayerIndex])
    {
        PlayerImages[PlayerIndex]->SetRenderTransformAngle(Rotation-90);
    }
}

void UWorldMapWidget::UpdateForceField(int32 ForceFieldIndex, float CurrentTime, float GrowthDuration)
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
            UE_LOG(LogTemp, Log, TEXT("ForceFieldCurrentSizes[ForceFieldIndex]: %f"), ForceFieldCurrentSizes[ForceFieldIndex]);
            CanvasSlot->SetSize(FVector2D(ForceFieldCurrentSizes[ForceFieldIndex], ForceFieldCurrentSizes[ForceFieldIndex]));
        }
    }
}

void UWorldMapWidget::SetForceFieldAlignment(int32 ForceFieldIndex, const FVector2D& WorldPosition)
{
    FVector2D MapPosition = ConvertWorldToMapPosition(WorldPosition);
    ForceFieldAlignments[ForceFieldIndex] = MapPosition;

    UImage* ForceFieldImages[4] = { ForceField1Image, ForceField2Image, ForceField3Image, ForceField4Image };
    if (ForceFieldImages[ForceFieldIndex])
    {
        UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ForceFieldImages[ForceFieldIndex]);
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(MapPosition);
            CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            UE_LOG(LogTemp, Log, TEXT("Position: %s"), *MapPosition.ToString());
        }
    }
}
