// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <EarthHero/GameSession/LobbyGameSession.h>

#include "EarthHero/EHGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "EarthHero/Character/Shooter/EHShooter.h"
#include "EarthHero/PlayerState/LobbyPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();

	CharacterClasses.SetNum(4);
	
	//Warrior
	//Mechanic
	//shooter 블루프린트
	static ConstructorHelpers::FClassFinder<AEHShooter> EHShooterAsset(TEXT("/Game/Blueprints/Character/Shooter/BP_Shooter.BP_Shooter_C"));
	if (EHShooterAsset.Succeeded())
	{
		CharacterClasses[Shooter] = EHShooterClass = EHShooterAsset.Class;
	}
	//Archor

	for(int i = 0; i < 4; i++)
		bSpotUsedArray.Add(false);
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	bUseSeamlessTravel = true;

	SpawnLocations.Add(FVector(500.0f, -150.0f, 0.0f));
	SpawnLocations.Add(FVector(500.0f, -50.0f, 0.0f));
	SpawnLocations.Add(FVector(500.0f, 50.0f, 0.0f));
	SpawnLocations.Add(FVector(500.0f, 150.0f, 0.0f));
}

//특정 스폰 지점 설정
AActor* ALobbyGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	int32 PlayerIndex = ControllerArray.IndexOfByKey(Player);
	
	if (PlayerIndex != INDEX_NONE)
	{
		ControllerArray.RemoveAt(PlayerIndex);
		LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
		PlayerNameArray.RemoveAt(PlayerIndex);
		PlayerReadyStateArray.RemoveAt(PlayerIndex);
		PlayerClassArray.RemoveAt(PlayerIndex);
		PlayerSpotArray.RemoveAt(PlayerIndex);
	}

	int PlayerStart = FindLobbyPlayerSpot();

	ControllerArray.Add(Player);
	LobbyPlayerControllerArray.Add(Cast<ALobbyPlayerController>(Player)); //...?
	PlayerNameArray.Add(Player->PlayerState->GetPlayerName());
	PlayerReadyStateArray.Add(false);
	PlayerClassArray.Add(Shooter); //임시
	PlayerSpotArray.Add(PlayerStart);
	
	return Super::FindPlayerStart_Implementation(Player, FString::FromInt(PlayerStart));
}

void ALobbyGameMode::AddPlayerInfo(ALobbyPlayerController* NewLobbyPlayerController)
{
	//클라이언트에게 기본 캐릭터를 골라주고
	//이름과 레디 리스트를 보내줌
	NewLobbyPlayerController->Client_SelectDefaultCharacter();
	UpdatePlayerNameListAndReadyState();
}

int ALobbyGameMode::FindLobbyPlayerSpot()
{
	ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(GameSession);
	if (LobbyGameSession)
	{
		int MaxNumberOfPlayers = LobbyGameSession->MaxNumberOfPlayersInSession;
		
		UE_LOG(LogTemp, Error, TEXT("Max n o p = %d"), MaxNumberOfPlayers);
		
		for(int i = 0; i < MaxNumberOfPlayers; i++)
		{
			if(!bSpotUsedArray[i])
			{
				bSpotUsedArray[i] = true;
				return i;
			}
		}
	}
	return 0;
}


void ALobbyGameMode::RemovePlayerInfo(const ALobbyPlayerController* ExitingLobbyPlayerController)
{
	if(ExitingLobbyPlayerController)
	{
		int32 PlayerIndex = LobbyPlayerControllerArray.IndexOfByKey(ExitingLobbyPlayerController);

		if (PlayerIndex != INDEX_NONE)
		{
			UE_LOG(LogTemp, Log, TEXT("Remove player %d information"), PlayerIndex);

			bSpotUsedArray[PlayerSpotArray[PlayerIndex]] = false;
			
			ExitingLobbyPlayerController->LobbyCharacter->Destroy();

			ControllerArray.RemoveAt(PlayerIndex);
			LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
			PlayerNameArray.RemoveAt(PlayerIndex);
			PlayerReadyStateArray.RemoveAt(PlayerIndex);
			PlayerClassArray.RemoveAt(PlayerIndex);
			PlayerSpotArray.RemoveAt(PlayerIndex);
		}
		else UE_LOG(LogTemp, Error, TEXT("Failed to find player information to remove"));

		//이름과 레디 리스트 갱신
		UpdatePlayerNameListAndReadyState();
	}
	else UE_LOG(LogTemp, Error, TEXT("Invaild NewLobbyPlayerController"));
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
	else UE_LOG(LogTemp, Error, TEXT("%s is not valid for player ready state"), *LobbyPlayerController->GetName());
}

//이름 리스트 갱신. UpdatePlayerReadyState()도 호출함
void ALobbyGameMode::UpdatePlayerNameListAndReadyState()
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
		//if (ReadyCount == (LobbyGameSession->MaxNumberOfPlayersInSession - 1)) //임시로 4명 아니라도 실행되게 함!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

//플레이어 캐릭터(클래스) 선택 업데이트
void ALobbyGameMode::UpdateCharacter(ALobbyPlayerController* LobbyPlayerController, EClassType ClassType)
{
	int PlayerNumber = LobbyPlayerControllerArray.Find(LobbyPlayerController);

	UE_LOG(LogTemp, Log, TEXT("Player %d : Update Character"), PlayerNumber);

	PlayerClassArray[PlayerNumber] = ClassType;

	//첫 생성이면 스폰 장소 찾기
	if(!(LobbyPlayerController->LobbyCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("First Spawn"));
	}
	else //아니라면 기존 캐릭터 제거
	{
		UE_LOG(LogTemp, Log, TEXT("Respawn"));
		LobbyPlayerController->LobbyCharacter->Destroy();
	}
	
	int SpawnSpotIndex = PlayerSpotArray[PlayerNumber];
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	//해당 자리에 캐릭터 생성
	switch (ClassType)
	{
		case Warrior:
			break;
		case Mechanic:
			break;
		case Shooter:
			LobbyPlayerController->LobbyCharacter = GetWorld()->SpawnActor<AEHShooter>(EHShooterClass, SpawnLocations[SpawnSpotIndex], FRotator(0.0f, 180.0f, 0.0f), SpawnParams);
			break;
		case Archer:
			break;
		default:
			break;
	}
	
	if (LobbyPlayerController->LobbyCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned character : %s"), *SpawnLocations[SpawnSpotIndex].ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character : %s"), *SpawnLocations[SpawnSpotIndex].ToString());
	}
}

void ALobbyGameMode::UpdateDifficulty(int Difficulty)
{
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Update difficulty : %d"), Difficulty);
		EHGameInstance->Difficulty = Difficulty;
	}

	for(ALobbyPlayerController* LobbyPlayerController : LobbyPlayerControllerArray)
	{
		if(LobbyPlayerController && !LobbyPlayerController->bHost)
			LobbyPlayerController->Client_UpdateDifficulty(Difficulty);
	}
}
