#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

UCLASS()
class EARTHHERO_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Level_Num;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameTimer_Tb;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Chat_Scr;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Chat_Etb;

	UPROPERTY(meta = (BindWidget))
	class UIngameHUDHeroUpgradeWidget* BP_IngameHUDHeroUpgrade;

	UPROPERTY(meta = (BindWidget))
	class UIngameHUDHeroUpgradeWidget* BP_IngameHUDHeroUpgrade_1;

	UPROPERTY(meta = (BindWidget))
	class UIngameHUDHeroUpgradeWidget* BP_IngameHUDHeroUpgrade_2;

	void InitializePlayerState(class UStatComponent* StatComponent);

	void UpdateGameTimer(int GameTimerSec);

	void SetIngameHUDHeroUpgrade(int Index, UTexture2D* UpgradeImage, UTexture2D* Level1Image, UTexture2D* Level2Image, UTexture2D* Level3Image, FText UpgradeName, FText UpgradeDetail);
	void ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	void AddChatMessage(const FText& Text);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	class UStatComponent* StatComponentRef;
};
