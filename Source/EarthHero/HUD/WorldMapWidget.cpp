#include "WorldMapWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "TimerManager.h"

void UWorldMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UpdateInterval = 0.2f;
    NumberOfPlayers = 4; // Default number of players, can be set dynamically

    StartUpdating();
}

void UWorldMapWidget::StartUpdating()
{
    // No need to start a timer for this refactor
}

void UWorldMapWidget::SetNumberOfPlayers(int32 NumPlayers)
{
    NumberOfPlayers = NumPlayers;
}

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
