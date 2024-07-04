// Fill out your copyright notice in the Description page of Project Settings.

#include "MainSessionGameMode.h"
#include <EarthHero/PlayerController/MainSessionPlayerController.h>
#include "UObject/ConstructorHelpers.h"

AMainSessionGameMode::AMainSessionGameMode()
{
	PlayerControllerClass = AMainSessionPlayerController::StaticClass();
}

void AMainSessionGameMode::BeginPlay()
{
	Super::BeginPlay();
}