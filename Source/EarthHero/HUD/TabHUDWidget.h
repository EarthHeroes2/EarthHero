#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamMemberWidget.h"
#include "BasicStatsWidget.h"
#include "StatusWidget.h"
#include "TabHUDWidget.generated.h"

UCLASS()
class EARTHHERO_API UTabHUDWidget : public UUserWidget
{
	GENERATED_BODY()
    
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

	// Functions to set values for each widget
	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetTeamMemberWidgetValues(int32 Index, FText Name, FText Class, FText PlayerNumber, int32 Level, float Exp, float HealthProgress);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetBasicStatsWidgetValues(int32 Index, FText KillScore, FText TotalDamage, FText TotalShield, FText TotalHeal);

	UFUNCTION(BlueprintCallable, Category = "TabHUD")
	void SetStatusWidgetValues(FText AttackDamage, FText AttackSpeed, FText AttackSkillDamage, FText AttackSkillCooltime, FText MoveSkillCooltime, FText CurrentHealth, FText CurrentShield, FText MaxHealth, FText HealthRegenerationRate, FText MoveSpeed);
};
