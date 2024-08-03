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

FVector2D UWorldMapWidget::ConvertPlayerWorldToMapPosition(const FVector2D& WorldPosition) const
{
    // Image Map dimensions
    float MapWidth = 689.5f;
    float MapHeight = 706.5f;

    // World map size and center (in world space)
    float MapLength = 403200.0f;
    float MapCenterX = 201600.0f;
    float MapCenterY = -201600.0f;

    // Adjusting the calculations to accommodate the new map center
    float MapX = (WorldPosition.Y - MapCenterY + MapLength / 2.0f) / MapLength * MapWidth - (MapWidth / 2.0f);
    float MapY = (MapHeight / 2.0f) - (WorldPosition.X - MapCenterX + MapLength / 2.0f) / MapLength * MapHeight;

    // Apply manual offsets
    MapX -= 12.0f;
    MapY -= 16.0f;

    return FVector2D(MapX, MapY);
}

void UWorldMapWidget::ToggleForceFields(bool bToggle)
{
    if(bToggle == false)
    {
        ForceField1Image->SetVisibility(ESlateVisibility::Hidden);
        ForceField2Image->SetVisibility(ESlateVisibility::Hidden);
        ForceField3Image->SetVisibility(ESlateVisibility::Hidden);
        ForceField4Image->SetVisibility(ESlateVisibility::Hidden);
    }
    if(bToggle == true)
    {
        ForceField1Image->SetVisibility(ESlateVisibility::Visible);
        ForceField2Image->SetVisibility(ESlateVisibility::Visible);
        ForceField3Image->SetVisibility(ESlateVisibility::Visible);
        ForceField4Image->SetVisibility(ESlateVisibility::Visible);
    }
}

FVector2D UWorldMapWidget::ConvertForceFieldWorldToMapPosition(const FVector2D& WorldPosition) const
{
    // Image Map dimensions
    float MapWidth = 689.5f;
    float MapHeight = 706.5f;

    // World map size and center (in world space)
    float MapLength = 403200.0f;
    float MapCenterX = 201600.0f;
    float MapCenterY = -201600.0f;

    // Adjusting the calculations to accommodate the new map center
    float MapX = (WorldPosition.Y - MapCenterY + MapLength / 2.0f) / MapLength * MapWidth - (MapWidth / 2.0f);
    float MapY = (MapHeight / 2.0f) - (WorldPosition.X - MapCenterX + MapLength / 2.0f) / MapLength * MapHeight;
    
    return FVector2D(MapX, MapY);
}

void UWorldMapWidget::SetPlayerPosition(int32 PlayerIndex, const FVector2D& Position)
{
    UImage* PlayerImages[4] = { Player1Image, Player2Image, Player3Image, Player4Image };

    if (PlayerImages[PlayerIndex])
    {
        FVector2D MapPosition = ConvertPlayerWorldToMapPosition(Position);
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
            CanvasSlot->SetSize(FVector2D(ForceFieldCurrentSizes[ForceFieldIndex], ForceFieldCurrentSizes[ForceFieldIndex]));
        }
    }
}

void UWorldMapWidget::SetForceFieldAlignment(int32 ForceFieldIndex, const FVector2D& WorldPosition)
{
    FVector2D MapPosition = ConvertForceFieldWorldToMapPosition(WorldPosition);
    ForceFieldAlignments[ForceFieldIndex] = MapPosition;

    UImage* ForceFieldImages[4] = { ForceField1Image, ForceField2Image, ForceField3Image, ForceField4Image };
    if (ForceFieldImages[ForceFieldIndex])
    {
        UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ForceFieldImages[ForceFieldIndex]);
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(MapPosition);
            CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

            float RotationAngle = -90.0f;
            switch (ForceFieldIndex)
            {
            case 0:
                RotationAngle = 270.0f;
                break;
            case 1:
                RotationAngle = 180.0f;
                break;
            case 2:
                RotationAngle = 90.0;
                break;
            case 3:
                RotationAngle = 0.0f;
                break;
            default:
                break;
            }
            ForceFieldImages[ForceFieldIndex]->SetRenderTransformAngle(RotationAngle);

            UE_LOG(LogTemp, Log, TEXT("Position: %s, Rotation: %f"), *MapPosition.ToString(), RotationAngle);
        }
    }
}
