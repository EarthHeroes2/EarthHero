#include "BasicStatsWidget.h"

void UBasicStatsWidget::SetKillScore(FText Value)
{
	if (KillScore_Text)
	{
		KillScore_Text->SetText(Value);
	}
}

void UBasicStatsWidget::SetTotalDamage(FText Value)
{
	if (TotalDamage_Text)
	{
		TotalDamage_Text->SetText(Value);
	}
}

void UBasicStatsWidget::SetTotalShield(FText Value)
{
	if (TotalShield_Text)
	{
		TotalShield_Text->SetText(Value);
	}
}

void UBasicStatsWidget::SetTotalHeal(FText Value)
{
	if (TotalHeal_Text)
	{
		TotalHeal_Text->SetText(Value);
	}
}
