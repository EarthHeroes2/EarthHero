#include "StatusWidget.h"
#include "Components/TextBlock.h"

void UStatusWidget::SetAttackDamage(FText Value)
{
	if (AttackDamageText)
	{
		AttackDamageText->SetText(Value);
	}
}

void UStatusWidget::SetAttackSpeed(FText Value)
{
	if (AttackSpeedText)
	{
		AttackSpeedText->SetText(Value);
	}
}

void UStatusWidget::SetAttackSkillDamage(FText Value)
{
	if (AttackSkillDamageText)
	{
		AttackSkillDamageText->SetText(Value);
	}
}

void UStatusWidget::SetAttackSkillCooltime(FText Value)
{
	if (AttackSkillCooltimeText)
	{
		AttackSkillCooltimeText->SetText(Value);
	}
}

void UStatusWidget::SetMoveSkillCooltime(FText Value)
{
	if (MoveSkillCooltimeText)
	{
		MoveSkillCooltimeText->SetText(Value);
	}
}

void UStatusWidget::SetCurrentHealth(FText Value)
{
	if (CurrentHealthText)
	{
		CurrentHealthText->SetText(Value);
	}
}

void UStatusWidget::SetCurrentShield(FText Value)
{
	if (CurrentShieldText)
	{
		CurrentShieldText->SetText(Value);
	}
}

void UStatusWidget::SetMaxHealth(FText Value)
{
	if (MaxHealthText)
	{
		MaxHealthText->SetText(Value);
	}
}

void UStatusWidget::SetHealthRegenerationRate(FText Value)
{
	if (HealthRegenerationRateText)
	{
		HealthRegenerationRateText->SetText(Value);
	}
}

void UStatusWidget::SetMoveSpeed(FText Value)
{
	if (MoveSpeedText)
	{
		MoveSpeedText->SetText(Value);
	}
}
