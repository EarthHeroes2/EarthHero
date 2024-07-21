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
    void SetPlayerPosition(int32 PlayerIndex, const FVector2D& Position);
    void SetPlayerRotation(int32 PlayerIndex, float Rotation);
    void UpdateForceField(int32 ForceFieldIndex, float CurrentTime, float GrowthDuration);
    void SetForceFieldAlignment(int32 ForceFieldIndex, const FVector2D& Alignment);

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

    UPROPERTY(meta = (BindWidget))
    UImage* ForceField1Image;

    UPROPERTY(meta = (BindWidget))
    UImage* ForceField2Image;

    UPROPERTY(meta = (BindWidget))
    UImage* ForceField3Image;

    UPROPERTY(meta = (BindWidget))
    UImage* ForceField4Image;

private:
    FVector2D ConvertWorldToMapPosition(const FVector2D& WorldPosition) const;

    int32 NumberOfPlayers;

    TArray<float> ForceFieldCurrentSizes;
    TArray<float> ForceFieldTargetSizes;
    TArray<float> ForceFieldGrowthRates;
    TArray<float> ForceFieldStartTimes;
    TArray<FVector2D> ForceFieldAlignments;

    float TempRotation = 0.f;
};
