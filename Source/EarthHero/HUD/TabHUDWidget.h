#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamMemberWidget.h"
#include "BasicStatsWidget.h"
#include "StatusWidget.h"
#include "HeroUpgradeWidget.h"
#include "TabHUDWidget.generated.h"

UCLASS()
class EARTHHERO_API UTabHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	TArray<UTeamMemberWidget*> TabUserInfoArray;
    
public:
	UPROPERTY(meta = (BindWidget))
	UTeamMemberWidget* BP_TabUserInfo_1;

	UPROPERTY(meta = (BindWidget))
	UTeamMemberWidget* BP_TabUserInfo_2;

	UPROPERTY(meta = (BindWidget))
	UTeamMemberWidget* BP_TabUserInfo_3;

	UPROPERTY(meta = (BindWidget))
	UTeamMemberWidget* BP_TabUserInfo_4;

	UPROPERTY(meta = (BindWidget))
	UBasicStatsWidget* BP_TabBasicStatsInfo_1;

	UPROPERTY(meta = (BindWidget))
	UBasicStatsWidget* BP_TabBasicStatsInfo_2;

	UPROPERTY(meta = (BindWidget))
	UBasicStatsWidget* BP_TabBasicStatsInfo_3;

	UPROPERTY(meta = (BindWidget))
	UBasicStatsWidget* BP_TabBasicStatsInfo_4;

	UPROPERTY(meta = (BindWidget))
	UStatusWidget* BP_TabStatusInfo;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_0;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_1;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_2;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_3;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_4;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_5;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_6;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_7;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_8;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_9;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_10;

	UPROPERTY(meta = (BindWidget))
	UHeroUpgradeWidget* BP_TabHeroUpgrade_11;

	void BeginPlay();
	// Functions to set values for each widget
	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetTeamMemberWidgetValues(int32 Index, FText Name, FText Class, FText PlayerNumber, int32 Level, float Exp, float HealthProgress);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetBasicStatsWidgetValues(int32 Index, FText KillScore, FText TotalDamage, FText TotalShield, FText TotalHeal);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetStatusWidgetValues(FText AttackDamage, FText AttackSpeed, FText AttackSkillDamage, FText AttackSkillCooltime, FText MoveSkillCooltime, FText CurrentHealth, FText CurrentShield, FText MaxHealth, FText HealthRegenerationRate, FText MoveSpeed);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetHeroUpgradeWidgetValues(int32 Index, UTexture2D* UpgradeImage, UTexture2D* Level1Image, UTexture2D* Level2Image, UTexture2D* Level3Image, FText UpgradeName, FText UpgradeDesc);


	void UpdatePlayerHealths(TArray<float> PlayerMaxHealths, TArray<float> PlayerCurrentHealths);
	void UpdatePlayerLevels(TArray<int> PlayerLevels);
	void UpdatePlayerExps(TArray<float> PlayerExps);
};
