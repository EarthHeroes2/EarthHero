// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamMemberWidget.h"

void UTeamMemberWidget::SetName(FText Name)
{
	if (NameText)
	{
		NameText->SetText(Name);
	}
}

void UTeamMemberWidget::SetClass(FText ClassName)
{
	if (ClassText)
	{
		ClassText->SetText(ClassName);
	}
}

void UTeamMemberWidget::SetPlayerNumber(FText PlayerNumber)
{
	if (PlayerNumberText)
	{
		PlayerNumberText->SetText(PlayerNumber);
	}
}

void UTeamMemberWidget::SetLevel(int32 Level)
{
	if (LevelText)
	{
		LevelText->SetText(FText::Format(FText::FromString("Lv.{0}"), FText::AsNumber(Level)));
	}
}

void UTeamMemberWidget::SetExp(float ExpPercentage)
{
	if (ExpText)
	{
		ExpText->SetText(FText::Format(FText::FromString(" / {0}%"), FText::AsNumber(ExpPercentage)));
	}
}

void UTeamMemberWidget::SetProgress(float Progress)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Progress);
	}
}