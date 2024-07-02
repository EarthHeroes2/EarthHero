#include "TabHUDWidget.h"

void UTabHUDWidget::SetTeamMemberWidgetValues(int32 Index, FText Name, FText Class, FText PlayerNumber, int32 Level, float Exp, float HealthProgress)
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
            return; // Invalid index
    }

    if (TeamMemberWidget)
    {
        TeamMemberWidget->SetName(Name);
        TeamMemberWidget->SetClass(Class);
        TeamMemberWidget->SetPlayerNumber(PlayerNumber);
        TeamMemberWidget->SetLevelAndExp(Level, Exp);
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
            return; // Invalid index
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
