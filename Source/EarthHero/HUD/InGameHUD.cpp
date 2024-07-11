// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

#include "EarthHero/Stat/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UInGameHUD::InitializePlayerState(UStatComponent *StatComponent)
{
	if (!StatComponent)
	{
		UE_LOG(LogClass, Warning, TEXT("InGameHUD: ERROR!! StatComponent is NULL"));
	}
	StatComponentRef = StatComponent;
}

void UInGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (StatComponentRef)
	{
		HealthBar->SetPercent(StatComponentRef->GetHealthPercent());
	}
}

void UInGameHUD::UpdateGameTimer(int GameTimerSec)
{
	GameTimer_Tb->SetText(FText::Format(FText::FromString("{0} : {1}"), FText::AsNumber(GameTimerSec/60), FText::AsNumber(GameTimerSec % 60)));
}