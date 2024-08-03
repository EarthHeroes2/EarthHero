// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundMonster.h"

#include "NavigationInvokerComponent.h"

AGroundMonster::AGroundMonster()
{
	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
	NavigationInvoker->SetGenerationRadii(5000.0f, 6000.0f); //생성, 제거 범위

	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
}
