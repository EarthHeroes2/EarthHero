// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameState.h"

#include "EarthHero/GameSession/PlayingGameSession.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void APlayingGameState::BeginPlay()
{
	Super::BeginPlay();

	if(!IsRunningDedicatedServer())
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if(PlayerController) EHPlayerController = Cast<AEHPlayerController>(PlayerController);
	}
}





void APlayingGameState::UpdateHUDGameTimer(int GameTimer)
{
	GameTimerSec = GameTimer;
}

void APlayingGameState::OnRep_GameTimerSec()
{
	UE_LOG(LogTemp, Log, TEXT("GameTimerSec updated to: %d"), GameTimerSec);
	if(EHPlayerController && EHPlayerController->HUD)
	{
		EHPlayerController->HUD->UpdateGameTimer(GameTimerSec);
	}
}

void APlayingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayingGameState, GameTimerSec);
}