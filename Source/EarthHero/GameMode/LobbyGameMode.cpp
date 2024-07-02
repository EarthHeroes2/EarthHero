// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <EarthHero/GameSession/LobbyGameSession.h>
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "EarthHero/Character/Shooter/EHShooter.h"
#include "Interfaces/OnlineSessionInterface.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();

	//shooter 블루프린트
	static ConstructorHelpers::FClassFinder<AEHShooter> EHShooterAsset(TEXT("/Game/Blueprints/Character/Shooter/BP_Shooter.BP_Shooter_C"));
	if (EHShooterAsset.Succeeded())
	{
		EHShooterClass = EHShooterAsset.Class;
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode<AGameModeBase>();
	if (IsValid(GameModeBase))
	{
		GameModeBase->bUseSeamlessTravel = true;
	}

	SpawnLocations.Add(FVector(500.0f, -150.0f, 0.0f));
	SpawnLocations.Add(FVector(500.0f, -50.0f, 0.0f));
	SpawnLocations.Add(FVector(500.0f, 50.0f, 0.0f));
	SpawnLocations.Add(FVector(500.0f, 150.0f, 0.0f));
}

void ALobbyGameMode::AddPlayerReadyState(APlayerController* NewPlayer)
{
	ALobbyPlayerController* LobbyNewPlayerController = Cast<ALobbyPlayerController>(NewPlayer);

	int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(LobbyNewPlayerController);

	if (PlayerIndex != INDEX_NONE)
	{
		LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
		PlayerNameArray.RemoveAt(PlayerIndex);
		PlayerReadyStateArray.RemoveAt(PlayerIndex);
		PlayerClassArray.RemoveAt(PlayerIndex);
	}

	LobbyPlayerControllerArray.Add(LobbyNewPlayerController);
	PlayerNameArray.Add(LobbyNewPlayerController->PlayerState->GetPlayerName());
	PlayerReadyStateArray.Add(false);
	PlayerClassArray.Add(Warrior); //�⺻��

	UpdatePlayerNameyListAndReadyState();
}

void ALobbyGameMode::TogglePlayerReady(APlayerController* Player)
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Player);

	int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(LobbyPlayerController);

	if (PlayerIndex != INDEX_NONE)
	{
		if (PlayerReadyStateArray[PlayerIndex]) ReadyCount--;
		else ReadyCount++;

		PlayerReadyStateArray[PlayerIndex] = !PlayerReadyStateArray[PlayerIndex];

		UpdatePlayerReadyState();
	}
	else UE_LOG(LogTemp, Error, TEXT("%s is not valid for player ready state"), LobbyPlayerController);
}

//��� Ŭ���̾�Ʈ���� �÷��̾� �̸� ����Ʈ ���� �� UpdatePlayerReadyState() ȣ��
void ALobbyGameMode::UpdatePlayerNameyListAndReadyState()
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Send player name list to client. (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_UpdatePlayerNameList(PlayerNameArray);
		}
	}

	UpdatePlayerReadyState();
}

//��� Ŭ���̾�Ʈ���� �ٲ� ���� ���� �迭 ����
void ALobbyGameMode::UpdatePlayerReadyState()
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();
	//ALobbyPlayerController�� for���� �ȵǳ���...

	UE_LOG(LogTemp, Log, TEXT("Send Ready state to client. (%d players)"), NumberOfPlayers);

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_UpdateReadyState(PlayerReadyStateArray);
		}
	}
}

bool ALobbyGameMode::PressGameStartButton()
{
	ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(GameSession);
	if (LobbyGameSession)
	{
		if (ReadyCount == (LobbyGameSession->MaxNumberOfPlayersInSession - 1))
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby Start!"));
			LobbyGameSession->StartSession();
			return true;
		}
	}
	return false;
}


void ALobbyGameMode::SendChatMessage(const FText& Text)
{
	int32 NumberOfPlayers = LobbyPlayerControllerArray.Num();

	UE_LOG(LogTemp, Log, TEXT("Send a message to clients"));

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (LobbyPlayerControllerArray[i])
		{
			LobbyPlayerControllerArray[i]->Client_SendChatMessage(Text);
		}
	}
}

//�ش� �÷��̾��� ĳ���͸� ����
void ALobbyGameMode::UpdateCharacter(ALobbyPlayerController* LobbyPlayerController, EClassType ClassType)
{
	int PlayerNumber = LobbyPlayerControllerArray.Find(LobbyPlayerController);

	PlayerClassArray[PlayerNumber] = ClassType;

	//캐릭터 첫 생성이라면
	if(!(LobbyPlayerController->LobbyCharacter))
	{
		int SpawnSpotIndex = GetLobbyPlayerSpot();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AEHShooter* NewCharacter = GetWorld()->SpawnActor<AEHShooter>(EHShooterClass, SpawnLocations[SpawnSpotIndex], FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
		//첫 소환은 슈터 고정
		if (NewCharacter)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawned character : %s"), *SpawnLocations[SpawnSpotIndex].ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character : %s"), *SpawnLocations[SpawnSpotIndex].ToString());
		}
	}
	else
	{
		
	}
}

int ALobbyGameMode::GetLobbyPlayerSpot()
{
	int NumberOfPlayer = LobbyPlayerControllerArray.Num();
	int i;
	
	for(i = 0; i < NumberOfPlayer; i++)
	{
		if(!(LobbyPlayerControllerArray[i]->bSpawnCharacter))
		{
			LobbyPlayerControllerArray[i]->bSpawnCharacter = true;
			break;
		}
	}
	
	return i;
}