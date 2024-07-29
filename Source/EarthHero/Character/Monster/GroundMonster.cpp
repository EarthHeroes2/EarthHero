// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundMonster.h"

#include "NavigationInvokerComponent.h"

AGroundMonster::AGroundMonster()
{
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
	NavigationInvoker->SetGenerationRadii(1500.0f, 2000.0f); //생성, 제거 범위
}
