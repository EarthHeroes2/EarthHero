#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamMemberWidget.h"
#include "BasicStatsWidget.h"
#include "StatusWidget.h"
#include "HeroUpgradeWidget.h"
#include "WorldMapWidget.h"
#include "TabHUDWidget.generated.h"

UCLASS()
class EARTHHERO_API UTabHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	TArray<UTeamMemberWidget*> TabUserInfoArray;
	TArray<UBasicStatsWidget*> TabUserBasicStatsArray;
    
public:
	UPROPERTY(meta = (BindWidget))
	UWorldMapWidget* BP_WorldMap;
	
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

	virtual bool Initialize() override;

	void SetTeamMemberWidgetValues(int32 Index, FString Name, EClassType ClassType, FText PlayerNumber, int32 Level, float Exp, float HealthProgress);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetBasicStatsWidgetValues(int32 Index, FText KillScore, FText TotalDamage, FText TotalShield, FText TotalHeal);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetStatusWidgetValues(FText AttackDamage, FText AttackSpeed, FText AttackSkillDamage, FText AttackSkillCooltime, FText MoveSkillCooltime, FText CurrentHealth, FText CurrentShield, FText MaxHealth, FText HealthRegenerationRate, FText MoveSpeed);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetHeroUpgradeWidgetValues(int32 Index, UTexture2D* UpgradeImage, int Level, FText UpgradeName, FText UpgradeDesc);

	void UpdatePlayerHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths);
	void UpdatePlayerLevels(const TArray<int>& PlayerLevels);
	void UpdatePlayerExps(const TArray<float>& PlayerExps);
	void UpdatePlayerClasses(const TArray<int>& PlayerClasses);
	void UpdatePlayerNames(const TArray<FString>& PlayerNames);
	void UpdateGameStateKillCount(const TArray<int> PlayerKillCount);
	void UpdateGameStateGivenDamage(const TArray<float> PlayerGivenDamage);
	void UpdateGameStateReceiveDamage(const TArray<float> PlayerReceiveDamage);
	void UpdateGameStateHeal(const TArray<float> PlayerHeal);
	
	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void UpdatePlayerImagesInWorldMap(const TArray<FVector2D>& PlayerPositions, const TArray<float>& PlayerRotations, int32 NumPlayers);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void UpdateForceField(int32 ForceFieldIndex, float CurrentTime, float GrowthDuration);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetForceFieldAlignment(int32 ForceFieldIndex, const FVector2D& Alignment);

private:
	TArray<FVector2D> InterpolatedPositions;
	TArray<float> InterpolatedRotations;

	TArray<FVector2D> TargetPositions;
	TArray<float> TargetRotations;

};
 