// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamMemberWidget.h"

void UTeamMemberWidget::SetName(const FText& Name)
{
	if (NameText)
	{
		NameText->SetText(Name);
	}
}

void UTeamMemberWidget::SetClass(const EClassType ClassType)
{
	if (ClassText)
	{
		switch (ClassType)
		{
		case Warrior:
			ClassText->SetText(FText::FromString("Warrior"));
			break;
		case Mechanic:
			ClassText->SetText(FText::FromString("Mechanic"));
			break;
		case Shooter:
			ClassText->SetText(FText::FromString("Shooter"));
			break;
		case Archer:
			ClassText->SetText(FText::FromString("Archer"));
			break;
		default: break;
		}
	}
}

void UTeamMemberWidget::SetPlayerNumber(const FText& PlayerNumber) const
{
	if (PlayerNumberText)
	{
		PlayerNumberText->SetText(PlayerNumber);
	}
}

void UTeamMemberWidget::SetLevel(const int32 Level) const
{
	if (LevelText)
	{
		LevelText->SetText(FText::Format(FText::FromString("Lv.{0}"), FText::AsNumber(Level)));
	}
}

void UTeamMemberWidget::SetExp(const float ExpPercentage) const
{
	if (ExpText)
	{
		ExpText->SetText(FText::Format(FText::FromString(" / {0}%"), FText::AsNumber(ExpPercentage)));
	}
}

void UTeamMemberWidget::SetProgress(const float Progress) const
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Progress);
	}
}
