// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameState.h"

#include "EarthHero/GameSession/PlayingGameSession.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "EarthHero/HUD/TabHUDWidget.h"
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





void APlayingGameState::UpdateHUDGameTimer(const int GameTimer)
{
	GameTimerSec = GameTimer;
}

void APlayingGameState::OnRep_GameTimerSec() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameTimerSec"));
	if(EHPlayerController && EHPlayerController->HUD)
	{
		EHPlayerController->HUD->UpdateGameTimer(GameTimerSec);
	}
}


void APlayingGameState::UpdateGameStateHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths)
{
	UE_LOG(LogTemp, Log, TEXT("UpdateGameStateHealths (gamestate)"));
	AllPlayerMaxHealths = PlayerMaxHealths;
	AllPlayerCurrentHealths = PlayerCurrentHealths;

	UE_LOG(LogTemp, Error , TEXT("Num = %d"), PlayerCurrentHealths.Num());
	for(float PlayerCurrentHealth : PlayerCurrentHealths)
	{
		UE_LOG(LogTemp, Error , TEXT("ch = %lf"), PlayerCurrentHealth);
	}
}

void APlayingGameState::OnRep_GameStateHealths() const //일단은 현재 체력이 변했을 때만 이것이 불림
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateHealths"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdatePlayerHealths(AllPlayerMaxHealths, AllPlayerCurrentHealths);
	}
}


void APlayingGameState::UpdateGameStateLevels(const TArray<int>& PlayerLevels)
{
	AllPlayerLevels = PlayerLevels;
}

void APlayingGameState::OnRep_GameStateLevels() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateLevels"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdatePlayerLevels(AllPlayerLevels);
	}
}


void APlayingGameState::UpdateGameStateExps(const TArray<float>& PlayerExps)
{
	AllPlayerExps = PlayerExps;
}

void APlayingGameState::OnRep_GameStateExps() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateExps"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdatePlayerExps(AllPlayerExps);
	}
}


void APlayingGameState::UpdateGameStateClasses(const TArray<int>& PlayerClasses)
{
	AllPlayerClasses = PlayerClasses;
}

void APlayingGameState::OnRep_GameStateClasses() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateClasses"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdatePlayerClasses(AllPlayerClasses);
	}
}


void APlayingGameState::UpdateGameStateNames(const TArray<FString>& PlayerNames)
{
	AllPlayerNames = PlayerNames;
}

void APlayingGameState::OnRep_GameStateNames() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateNames"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdatePlayerNames(AllPlayerNames);
	}
}

void APlayingGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayingGameState, GameTimerSec);
	DOREPLIFETIME(APlayingGameState, AllPlayerCurrentHealths);
	DOREPLIFETIME(APlayingGameState, AllPlayerLevels);
	DOREPLIFETIME(APlayingGameState, AllPlayerExps);
	DOREPLIFETIME(APlayingGameState, AllPlayerClasses);
	DOREPLIFETIME(APlayingGameState, AllPlayerNames);
}
