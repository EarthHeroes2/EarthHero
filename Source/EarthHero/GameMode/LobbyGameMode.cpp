#include "LobbyGameMode.h"
#include <EarthHero/GameSession/LobbyGameSession.h>

#include "EarthHero/EHGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "EarthHero/Character/Shooter/EHShooter.h"
#include "EarthHero/Character/Warrior/EHWarrior.h"
#include "EarthHero/PlayerState/LobbyPlayerState.h"
#include "EarthHero/Socket/SocketClient.h"

ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();

	CharacterClasses.SetNum(4);
	
	//Warrior
	static ConstructorHelpers::FClassFinder<AEHWarrior> EHWarriorAsset(TEXT("/Game/Blueprints/Character/Warrior/BP_Warrior.BP_Warrior_C"));
	if (EHWarriorAsset.Succeeded()) CharacterClasses[Warrior] = EHWarriorClass = EHWarriorAsset.Class;

	//Mechanic

	//shooter
	static ConstructorHelpers::FClassFinder<AEHShooter> EHShooterAsset(TEXT("/Game/Blueprints/Character/Shooter/BP_Shooter.BP_Shooter_C"));
	if (EHShooterAsset.Succeeded()) CharacterClasses[Shooter] = EHShooterClass = EHShooterAsset.Class;

	//Archor

	for(int i = 0; i < 4; i++) bSpotUsedArray.Add(false);

	SpawnLocations.Add(FVector(2640.0f, 1420.0f, -406.744093));
	SpawnLocations.Add(FVector(2230.0f, 1360.0f, -397.733997));
	SpawnLocations.Add(FVector(2340.0f, 930.0f, -54.579708));
	SpawnLocations.Add(FVector(2603.821996, 959.925651, -70.079027));

	SpawnRotations.Add(FRotator(0.0f, 50.0f, 0.0f));
	SpawnRotations.Add(FRotator(0.0f, 119.999999, 0.0f));
	SpawnRotations.Add(FRotator(0.0f, 159.999999, 0.0f));
	SpawnRotations.Add(FRotator(0.0f, 10.0f, 0.0f));
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	bUseSeamlessTravel = true;
}

int ALobbyGameMode::GetPlayerLevel(FUniqueNetIdRepl UniqueNetId)
{
	FString SteamId = UniqueNetId->ToString();
	UE_LOG(LogTemp, Log, TEXT("My Steam ID: %s"), *SteamId);
	
	FString PlayerLevelString;
	
	USocketClient* NewSocket = NewObject<USocketClient>(this);
	if(NewSocket) PlayerLevelString = NewSocket->CreateSocket("GetPlayerLevel", SteamId);

	if(PlayerLevelString == "")
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to GetPlayerLevel"));
		return -1;
	}
	
	return FCString::Atoi(*PlayerLevelString);
}

//특정 스폰 지점 설정
AActor* ALobbyGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	CallCount++;

	if(CallCount % 2 == 0) //게임모드 베이스의 RestartPlayer에서 호출되어 왔을 것임
	{
		int32 PlayerIndex = ControllerArray.IndexOfByKey(Player);
	
		if (PlayerIndex != INDEX_NONE)
		{
			ControllerArray.RemoveAt(PlayerIndex);
			LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
			PlayerNameArray.RemoveAt(PlayerIndex);
			PlayerLevelArray.RemoveAt(PlayerIndex);
			PlayerReadyStateArray.RemoveAt(PlayerIndex);
			PlayerClassArray.RemoveAt(PlayerIndex);
			PlayerSpotArray.RemoveAt(PlayerIndex);
		}

		int PlayerStart = FindLobbyPlayerSpot();

		ControllerArray.Add(Player);
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(Player); //임시
		LobbyPlayerControllerArray.Add(LobbyPlayerController);
		PlayerNameArray.Add(Player->PlayerState->GetPlayerName());
		PlayerLevelArray.Add(GetPlayerLevel(Player->PlayerState->GetUniqueId()));
		PlayerReadyStateArray.Add(false);
		PlayerClassArray.Add(Warrior); //ALobbyPlayerController::Client_SelectDefaultCharacter_Implementation() 기본값 변경이면 여기도 함께 변경해줘야함 (나중에 수정할수도)
		PlayerSpotArray.Add(PlayerStart);

		return Super::FindPlayerStart_Implementation(Player, FString::FromInt(PlayerStart));
	}
	return Super::FindPlayerStart_Implementation(Player, "");
}

void ALobbyGameMode::JoinedPlayerInitSetup(ALobbyPlayerController* NewLobbyPlayerController)
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

void ALobbyGameMode::Kick(int PlayerNumber)
{
	if(LobbyPlayerControllerArray[PlayerNumber])
	{
		UE_LOG(LogTemp, Log, TEXT("Player %d Kicked"), PlayerNumber);
		LobbyPlayerControllerArray[PlayerNumber]->ClientTravel("/Game/Maps/StartupMap", ETravelType::TRAVEL_Absolute);
	}
	else UE_LOG(LogTemp, Log, TEXT("Failed to %d Kick"), PlayerNumber);
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
			
			ExitingLobbyPlayerController->DestroyCharacter();

			ControllerArray.RemoveAt(PlayerIndex);
			LobbyPlayerControllerArray.RemoveAt(PlayerIndex);
			PlayerNameArray.RemoveAt(PlayerIndex);
			PlayerLevelArray.RemoveAt(PlayerIndex);
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

void ALobbyGameMode::UpdateCharacter(ALobbyPlayerController* LobbyPlayerController, EClassType ClassType)
{
	int PlayerNumber = LobbyPlayerControllerArray.Find(LobbyPlayerController);

	UE_LOG(LogTemp, Log, TEXT("Player %d : Update Character"), PlayerNumber);

	PlayerClassArray[PlayerNumber] = ClassType;

	//이전 캐릭터 있으면 지움
	LobbyPlayerController->DestroyCharacter();
    
	int SpawnSpotIndex = PlayerSpotArray[PlayerNumber];
    
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; // 항상 생성

	//해당 자리에 캐릭터 생성
	switch (ClassType)
	{
	case Warrior:
		LobbyPlayerController->SetCharacter(GetWorld()->SpawnActor<AEHWarrior>(EHWarriorClass, SpawnLocations[SpawnSpotIndex], SpawnRotations[SpawnSpotIndex], SpawnParams)); // Use rotation
		break;
	case Mechanic:
		break;
	case Shooter:
		LobbyPlayerController->SetCharacter(GetWorld()->SpawnActor<AEHShooter>(EHShooterClass, SpawnLocations[SpawnSpotIndex], SpawnRotations[SpawnSpotIndex], SpawnParams)); // Use rotation
		break;
	case Archer:
		break;
	default:
		break;
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
		if(LobbyPlayerController)
			LobbyPlayerController->Client_UpdateDifficulty(Difficulty);
	}
}

ALobbyPlayerController* ALobbyGameMode::GetFirstLobbyPlayerController()
{
	if(LobbyPlayerControllerArray.Num() > 0) return LobbyPlayerControllerArray[0];
	
	return nullptr;
}
