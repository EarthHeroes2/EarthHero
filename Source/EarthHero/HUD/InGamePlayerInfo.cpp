// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerInfo.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UInGamePlayerInfo::SetName(const FString& Name) const
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(Name));
	}
}

void UInGamePlayerInfo::SetProgress(const float Progress) const
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Progress);
	}
}

void UInGamePlayerInfo::SetImage(FSlateBrush SlateBrush) const
{
	PlayerClass->SetBrush(SlateBrush);
}

bool UInGamePlayerInfo::Initialize()
{
	Super::Initialize();

	PLayerStatusArray.Add(Player_Status);
	PLayerStatusArray.Add(Player_Status_1);
	PLayerStatusArray.Add(Player_Status_2);
	PLayerStatusArray.Add(Player_Status_3);
	PLayerStatusArray.Add(Player_Status_4);
	PLayerStatusArray.Add(Player_Status_5);
	PLayerStatusArray.Add(Player_Status_6);
	PLayerStatusArray.Add(Player_Status_7);
	PLayerStatusArray.Add(Player_Status_8);
	PLayerStatusArray.Add(Player_Status_9);
	PLayerStatusArray.Add(Player_Status_10);

	return true;
}
