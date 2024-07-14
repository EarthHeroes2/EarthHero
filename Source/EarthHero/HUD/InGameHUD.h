#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHUD.generated.h"

UCLASS()
class EARTHHERO_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	//07 13 체력 /최대 체력 텍스트 연동
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHealthText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHealthText;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Exp_Num;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Level_Num;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameTimer_Tb;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Chat_Scr;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Chat_Etb;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* HeroUpgradeVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UIngameHUDHeroUpgradeWidget* BP_IngameHUDHeroUpgrade;

	UPROPERTY(meta = (BindWidget))
	class UIngameHUDHeroUpgradeWidget* BP_IngameHUDHeroUpgrade_1;

	UPROPERTY(meta = (BindWidget))
	class UIngameHUDHeroUpgradeWidget* BP_IngameHUDHeroUpgrade_2;

	void ShowHeroUpgradeVerticalBox();

	void HideHeroUpgradeVerticalBox();
	
	void InitializePlayerState(class UStatComponent* StatComponent);

	void UpdateGameTimer(int GameTimerSec);

	void SetIngameHUDHeroUpgrade(int Index, UTexture2D* UpgradeImage, int Level, FText UpgradeName, FText UpgradeDetail);
	
	void AddChatMessage(const FText& Text);

	//플레이어 컨트롤러가 히어로 업그레이드 선택할 수 있는 환경인지 확인하는 변수
	bool IsHeroUpgradeReadys() const;
	
	bool IsHeroUpGradeReady[3] = {false, };

	UFUNCTION()
	void SetFalseHeroUpgradeReady();
protected:
	UFUNCTION()
	void ChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	class UStatComponent* StatComponentRef;
};
