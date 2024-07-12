#include "TabHUDWidget.h"

//#include "BehaviorTree/BehaviorTreeTypes.h" <- 현재 이거 있으면 fatal 에러남 - 박정익


bool UTabHUDWidget::Initialize()
{
    Super::Initialize();
    
    TabUserInfoArray.Add(BP_TabUserInfo_1);
    TabUserInfoArray.Add(BP_TabUserInfo_2);
    TabUserInfoArray.Add(BP_TabUserInfo_3);
    TabUserInfoArray.Add(BP_TabUserInfo_4);

    UE_LOG(LogTemp, Log, TEXT("TAB UHD!!!!!!!!!!!!!!!!!!!!!!!"));

    return true;
}

void UTabHUDWidget::SetTeamMemberWidgetValues(int32 Index, FString Name, EClassType ClassType, FText PlayerNumber, int32 Level, float Exp, float HealthProgress)
{
    UTeamMemberWidget* TeamMemberWidget = nullptr;

    switch(Index)
    {
        case 1:
            TeamMemberWidget = BP_TabUserInfo_1;
            break;
        case 2:
            TeamMemberWidget = BP_TabUserInfo_2;
            break;
        case 3:
            TeamMemberWidget = BP_TabUserInfo_3;
            break;
        case 4:
            TeamMemberWidget = BP_TabUserInfo_4;
            break;
        default:
            return;
    }

    if (TeamMemberWidget)
    {
        TeamMemberWidget->SetName(Name);
        TeamMemberWidget->SetClass(ClassType);
        TeamMemberWidget->SetPlayerNumber(PlayerNumber);
        TeamMemberWidget->SetLevel(Level);
        TeamMemberWidget->SetExp(Exp);
        TeamMemberWidget->SetProgress(HealthProgress);
    }
}

void UTabHUDWidget::SetBasicStatsWidgetValues(int32 Index, FText KillScore, FText TotalDamage, FText TotalShield, FText TotalHeal)
{
    UBasicStatsWidget* BasicStatsWidget = nullptr;

    switch(Index)
    {
        case 1:
            BasicStatsWidget = BP_TabBasicStatsInfo_1;
            break;
        case 2:
            BasicStatsWidget = BP_TabBasicStatsInfo_2;
            break;
        case 3:
            BasicStatsWidget = BP_TabBasicStatsInfo_3;
            break;
        case 4:
            BasicStatsWidget = BP_TabBasicStatsInfo_4;
            break;
        default:
            return;
    }

    if (BasicStatsWidget)
    {
        BasicStatsWidget->SetKillScore(KillScore);
        BasicStatsWidget->SetTotalDamage(TotalDamage);
        BasicStatsWidget->SetTotalShield(TotalShield);
        BasicStatsWidget->SetTotalHeal(TotalHeal);
    }
}

void UTabHUDWidget::SetStatusWidgetValues(FText AttackDamage, FText AttackSpeed, FText AttackSkillDamage, FText AttackSkillCooltime, FText MoveSkillCooltime, FText CurrentHealth, FText CurrentShield, FText MaxHealth, FText HealthRegenerationRate, FText MoveSpeed)
{
    if (BP_TabStatusInfo)
    {
        BP_TabStatusInfo->SetAttackDamage(AttackDamage);
        BP_TabStatusInfo->SetAttackSpeed(AttackSpeed);
        BP_TabStatusInfo->SetAttackSkillDamage(AttackSkillDamage);
        BP_TabStatusInfo->SetAttackSkillCooltime(AttackSkillCooltime);
        BP_TabStatusInfo->SetMoveSkillCooltime(MoveSkillCooltime);
        BP_TabStatusInfo->SetCurrentHealth(CurrentHealth);
        BP_TabStatusInfo->SetCurrentShield(CurrentShield);
        BP_TabStatusInfo->SetMaxHealth(MaxHealth);
        BP_TabStatusInfo->SetHealthRegenerationRate(HealthRegenerationRate);
        BP_TabStatusInfo->SetMoveSpeed(MoveSpeed);
    }
}

void UTabHUDWidget::SetHeroUpgradeWidgetValues(int32 Index, UTexture2D* UpgradeImage, UTexture2D* Level1Image, UTexture2D* Level2Image, UTexture2D* Level3Image, FText UpgradeName, FText UpgradeDesc)
{
    UHeroUpgradeWidget* HeroUpgradeWidget = nullptr;

    switch(Index)
    {
        case 0:
            HeroUpgradeWidget = BP_TabHeroUpgrade_0;
            break;
        case 1:
            HeroUpgradeWidget = BP_TabHeroUpgrade_1;
            break;
        case 2:
            HeroUpgradeWidget = BP_TabHeroUpgrade_2;
            break;
        case 3:
            HeroUpgradeWidget = BP_TabHeroUpgrade_3;
            break;
        case 4:
            HeroUpgradeWidget = BP_TabHeroUpgrade_4;
            break;
        case 5:
            HeroUpgradeWidget = BP_TabHeroUpgrade_5;
            break;
        case 6:
            HeroUpgradeWidget = BP_TabHeroUpgrade_6;
            break;
        case 7:
            HeroUpgradeWidget = BP_TabHeroUpgrade_7;
            break;
        case 8:
            HeroUpgradeWidget = BP_TabHeroUpgrade_8;
            break;
        case 9:
            HeroUpgradeWidget = BP_TabHeroUpgrade_9;
            break;
        case 10:
            HeroUpgradeWidget = BP_TabHeroUpgrade_10;
            break;
        case 11:
            HeroUpgradeWidget = BP_TabHeroUpgrade_11;
            break;
        default:
            return;
    }

    if (HeroUpgradeWidget)
    {
        HeroUpgradeWidget->SetUpgradeImage(UpgradeImage);
        HeroUpgradeWidget->SetLevel1Image(Level1Image);
        HeroUpgradeWidget->SetLevel2Image(Level2Image);
        HeroUpgradeWidget->SetLevel3Image(Level3Image);
        HeroUpgradeWidget->SetUpgradeName(UpgradeName);
        HeroUpgradeWidget->SetUpgradeDesc(UpgradeDesc);
    }
}

void UTabHUDWidget::UpdatePlayerHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths)
{
    for(int i = 0; i < PlayerMaxHealths.Num(); i++)
        TabUserInfoArray[i]->SetProgress(PlayerMaxHealths[i] / PlayerCurrentHealths[i]);
}

void UTabHUDWidget::UpdatePlayerLevels(const TArray<int>& PlayerLevels)
{
    for(int i = 0; i < PlayerLevels.Num(); i++)
        TabUserInfoArray[i]->SetLevel(PlayerLevels[i]);
}

void UTabHUDWidget::UpdatePlayerExps(const TArray<float>& PlayerExps)
{
    for(int i = 0; i < PlayerExps.Num(); i++)
        TabUserInfoArray[i]->SetExp(PlayerExps[i]);
}

void UTabHUDWidget::UpdatePlayerClasses(const TArray<int>& PlayerClasses)
{
    for(int i = 0; i < PlayerClasses.Num(); i++)
        TabUserInfoArray[i]->SetClass(static_cast<EClassType>(PlayerClasses[i]));
}

void UTabHUDWidget::UpdatePlayerNames(const TArray<FString>& PlayerNames)
{
    for(int i = 0; i < PlayerNames.Num(); i++)
        TabUserInfoArray[i]->SetName(PlayerNames[i]);
}
