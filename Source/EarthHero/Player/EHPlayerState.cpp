// Fill out your copyright notice in the Description page of Project Settings.


#include "EHPlayerState.h"

#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/StatComponent.h"

AEHPlayerState::AEHPlayerState()
{
	//승언 : StatComponent 붙이기
	
	StatComponent = CreateDefaultSubobject<UShooterStatComponent>(TEXT("ShooterStatComponent"));
}
