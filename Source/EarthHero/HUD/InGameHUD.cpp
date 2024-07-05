// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

#include "EarthHero/Stat/StatComponent.h"
#include "Components/ProgressBar.h"

void UInGameHUD::InitializePlayerState(UStatComponent *StatComponent)
{
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
