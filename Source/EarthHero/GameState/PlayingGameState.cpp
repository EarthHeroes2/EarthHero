// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameState.h"

#include "EarthHero/Character/EHCharacter.h"
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

	//Sec에 따라 forcefield 업데이트
	if (EHPlayerController && EHPlayerController->TabHUD && AllExpansionDurations.IsValidIndex(0))
	{
		for (int index = 0; index <= 3; index++)
		{
			EHPlayerController->TabHUD->UpdateForceField(index, GameTimerSec, AllExpansionDurations[index]);
		}
	}
}

void APlayingGameState::UpdateGameStateHealths(const TArray<float>& PlayerMaxHealths, const TArray<float>& PlayerCurrentHealths)
{
	AllPlayerMaxHealths = PlayerMaxHealths;
	AllPlayerCurrentHealths = PlayerCurrentHealths;
}

void APlayingGameState::OnRep_GameStateHealths() const //일단은 현재 체력이 변했을 때만 이것이 불림
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateHealths"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdatePlayerHealths(AllPlayerMaxHealths, AllPlayerCurrentHealths);
	}

	if(EHPlayerController && EHPlayerController->HUD)
	{
		EHPlayerController->HUD->UpdatePlayerHealths(AllPlayerMaxHealths, AllPlayerCurrentHealths);
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

void APlayingGameState::UpdateGameStateKillCount(const TArray<int> PlayerKillCount)
{
	AllPlayerKillCount = PlayerKillCount;
}

void APlayingGameState::OnRep_GameStateKillCount() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateKillCount"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdateGameStateKillCount(AllPlayerKillCount);
	}
}

void APlayingGameState::UpdateGameStateGivenDamage(const TArray<float> PlayerGivenDamage)
{
	AllPlayerGivenDamage = PlayerGivenDamage;
}

void APlayingGameState::OnRep_GameStateGivenDamage() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateGivenDamage"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdateGameStateGivenDamage(AllPlayerGivenDamage);
	}
}

void APlayingGameState::UpdateGameStateReceiveDamage(const TArray<float> PlayerReceiveDamage)
{
	AllPlayerReceiveDamage = PlayerReceiveDamage;
}

void APlayingGameState::OnRep_GameStateReceiveDamage() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateReceiveDamage"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdateGameStateReceiveDamage(AllPlayerReceiveDamage);
	}
}

void APlayingGameState::UpdateGameStateHeal(const TArray<float> PlayerHeal)
{
	AllPlayerHeal = PlayerHeal;
}

void APlayingGameState::OnRep_GameStateHeal() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateHeal"));
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		EHPlayerController->TabHUD->UpdateGameStateHeal(AllPlayerHeal);
	}
}

void APlayingGameState::UpdateGameStateWorldMaps(const TArray<FVector2D> ActorLocations, const TArray<float> ActorRotations,
	const int32 PlayerNumbers)
{
	AllActorLocations = ActorLocations;
	AllActorRotations = ActorRotations;
	AllPlayerNumbers = PlayerNumbers;
}

void APlayingGameState::OnRep_GameStateActorLocations() const
{
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateActorLocations"));
		EHPlayerController->TabHUD->UpdatePlayerImagesInWorldMap(AllActorLocations, AllActorRotations, AllPlayerNumbers);
	}
}

void APlayingGameState::OnRep_GameStateActorRotations() const
{
	if(EHPlayerController && EHPlayerController->TabHUD)
	{
		//UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateActorRotations"));
		EHPlayerController->TabHUD->UpdatePlayerImagesInWorldMap(AllActorLocations, AllActorRotations, AllPlayerNumbers);
	}
}

void APlayingGameState::OnRep_GameStateExpansionDurations() const
{
}

void APlayingGameState::OnRep_GameStateForceFieldLocations() const
{
	UE_LOG(LogTemp, Log, TEXT("OnRep_GameStateForceFieldLocations"));
	if (EHPlayerController && EHPlayerController->TabHUD)
	{
		UE_LOG(LogTemp, Log, TEXT("EHPlayerController && EHPlayerController->TabHUD"));
		for (int index = 0; index <= 3; index++)
		{
			UE_LOG(LogTemp, Log, TEXT("junmoon"));
			EHPlayerController->TabHUD->SetForceFieldAlignment(index, AllForceFieldLocations[index]);
		}
	}
}

void APlayingGameState::SetGameStateForceField(const TArray<float> ExpansionDurations, const TArray<FVector2D> ForceFieldLocations)
{
	UE_LOG(LogTemp, Log, TEXT("GameStateForceFieldLocations : %d"), ForceFieldLocations.Num());
	AllExpansionDurations = ExpansionDurations;
	AllForceFieldLocations = ForceFieldLocations;
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
	DOREPLIFETIME(APlayingGameState, AllPlayerKillCount);
	DOREPLIFETIME(APlayingGameState, AllPlayerGivenDamage);
	DOREPLIFETIME(APlayingGameState, AllPlayerReceiveDamage);
	DOREPLIFETIME(APlayingGameState, AllPlayerHeal);
	DOREPLIFETIME(APlayingGameState, AllActorLocations);
	DOREPLIFETIME(APlayingGameState, AllActorRotations);
	DOREPLIFETIME(APlayingGameState, AllPlayerNumbers);
	DOREPLIFETIME(APlayingGameState, AllExpansionDurations);
	DOREPLIFETIME(APlayingGameState, AllForceFieldLocations);
}
