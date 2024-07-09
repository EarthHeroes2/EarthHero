#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Options.generated.h"

class UComboBoxString;
class USlider;

UCLASS()
class EARTHHERO_API UOptions : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ScreenModeComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* MaxFrameComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* VSyncComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* OverallQualityComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* AntiAliasingComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* PostProcessingComboBox;

    UPROPERTY(meta = (BindWidget))
    USlider* MasterVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* BackgroundVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* SFXVolumeSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* MouseSensitivitySlider;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseBtn;
    
    void SetRecommendedResolution();

    static bool ParseResolution(const FString& ResolutionString, int32& Width, int32& Height);

    void InitializeOptions();

private:
    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnScreenModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnMaxFrameChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnVSyncChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnOverallQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnAntiAliasingChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnPostProcessingChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION()
    void OnMasterVolumeChanged(float Value);

    UFUNCTION()
    void OnBackgroundVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXVolumeChanged(float Value);

    UFUNCTION()
    void OnMouseSensitivityChanged(float Value);

    UFUNCTION()
    void OnCloseBtnClicked();

    FString QualityToString(int32 Quality);
    int32 QualityFromString(const FString& QualityString);

    FString ScreenModeToString(int32 ScreenMode);
    int32 ScreenModeFromString(const FString& ScreenModeString);
};
