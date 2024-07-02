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

void UTeamMemberWidget::SetLevelAndExp(int32 Level, float ExpPercentage)
{
	if (LevelAndExpText)
	{
		// Format the text as "Lv.<Level> / <ExpPercentage>%"
		FText LevelAndExp = FText::Format(FText::FromString("Lv.{0} / {1}%"), FText::AsNumber(Level), FText::AsNumber(ExpPercentage));
		LevelAndExpText->SetText(LevelAndExp);
	}
}

void UTeamMemberWidget::SetProgress(float Progress)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Progress);
	}
}