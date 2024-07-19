#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "WorldMapWidget.generated.h"

UCLASS()
class EARTHHERO_API UWorldMapWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    virtual void NativeConstruct() override;

    void SetPlayerPosition(int32 PlayerIndex, const FVector2D& Position);
    void SetPlayerRotation(int32 PlayerIndex, float Rotation);
    void SetNumberOfPlayers(int32 NumPlayers);

    UPROPERTY(meta = (BindWidget))
    UImage* WorldMapImage;

    UPROPERTY(meta = (BindWidget))
    UImage* Player1Image;

    UPROPERTY(meta = (BindWidget))
    UImage* Player2Image;

    UPROPERTY(meta = (BindWidget))
    UImage* Player3Image;

    UPROPERTY(meta = (BindWidget))
    UImage* Player4Image;

private:
    FVector2D ConvertWorldToMapPosition(const FVector2D& WorldPosition) const;

    float UpdateInterval;
    FTimerHandle UpdateTimerHandle;

    void StartUpdating();
    int32 NumberOfPlayers;
};
