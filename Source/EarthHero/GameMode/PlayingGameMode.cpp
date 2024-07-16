// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameMode.h"

#include "EarthHero/ForceField/ForceField.h"
#include "EarthHero/GameSession/PlayingGameSession.h"
#include "EarthHero/GameState/PlayingGameState.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "EarthHero/Player/EHPlayerState.h"


void APlayingGameMode::BeginPlay()
{
	Super::BeginPlay();

	bUseSeamlessTravel = true;
	
	SpawnForceFields();
}

void APlayingGameMode::SpawnForceFields()
{
	float MinX = -100800.0f;
	float MaxX = 75600.0f;
	float MinY = -100800.0f;
	float MaxY = 75600.0f;

	UWorld* World = GetWorld();
	if (World)
	{
		// Top side (Y = MaxY)
		FVector TopLocation = FVector(FMath::RandRange(MinX, MaxX), MaxY, 0.0f);
		SpawnForceFieldAtLocation(TopLocation);

		// Bottom side (Y = MinY)
		FVector BottomLocation = FVector(FMath::RandRange(MinX, MaxX), MinY, 0.0f);
		SpawnForceFieldAtLocation(BottomLocation);

		// Right side (X = MaxX)
		FVector RightLocation = FVector(MaxX, FMath::RandRange(MinY, MaxY), 0.0f);
		SpawnForceFieldAtLocation(RightLocation);

		// Left side (X = MinX)
		FVector LeftLocation = FVector(MinX, FMath::RandRange(MinY, MaxY), 0.0f);
		SpawnForceFieldAtLocation(LeftLocation);
	}
}

void APlayingGameMode::SpawnForceFieldAtLocation(FVector Location)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString AssetPath = TEXT("Blueprint'/Game/Blueprints/ForceField/BP_ForceField.BP_ForceField_C'");
		UClass* ForceFieldClass = StaticLoadClass(AActor::StaticClass(), nullptr, *AssetPath);
		if (ForceFieldClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FRotator SpawnRotation = FRotator::ZeroRotator;
			AActor* SpawnedForceField = World->SpawnActor<AActor>(ForceFieldClass, Location, SpawnRotation, SpawnParams);
			if (SpawnedForceField)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawned ForceField at location: %s"), *Location.ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load ForceField Blueprint class"));
		}
	}
}

//심레스 트래블 이후 새로운 컨트롤러 생김
void APlayingGameMode::InitSeamlessTravelPlayer(AController* NewController) //아니면... GenericPlayerInitialization?
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	APlayerController* NewPlayerController = Cast<APlayerController>(NewController);

	if(NewPlayerController)
	{
		AEHPlayerController* NewEHPlayerController = Cast<AEHPlayerController>(NewController);
		if(NewEHPlayerController)
		{
			APlayingGameSession* PlayingGameSession = Cast<APlayingGameSession>(GameSession);
			if (PlayingGameSession)
			{
				EHPlayerControllers.Add(NewEHPlayerController);
				
				//세션 속 모든 플레이어가 레벨에 들어왔다면 레벨 초기 작업 시작
				if(EHPlayerControllers.Num() == PlayingGameSession->GetNumPlayersInSession()) InitLevelSetting();
			}
		}
	}
}

void APlayingGameMode::InitLevelSetting()
{
	for(int i = 0; i < EHPlayerControllers.Num(); i++)
	{
		//플레이어 시작위치를 정해줄 수는 있는데.. 코드를 보면 보이지는 않지만 폰이 2개 생성되는 것 같음 (보류)
		//AActor* TargetPlayerStart = FindPlayerStart(EHPlayerControllers[i], FString::FromInt(i));
		//RestartPlayerAtPlayerStart(EHPlayerControllers[i], TargetPlayerStart);
	}
}

void APlayingGameMode::PlayerControllerReady() //조금 느리지만 안전하게 다 확인하고
{
	NumPlayerControllerReady++;
	
	//모든 플레이어가 준비 완료되었다면
	if(NumPlayerControllerReady == EHPlayerControllers.Num())
	{
		UpdateGameStateNames();
		UpdateGameStateClasses();
		UpdateGameStateExps();
		UpdateGameStateLevels();
		UpdateGameStateHealths();
	 
		//모두의 움직임 풀어줌
		EnableAllInput();

		//게임 시간은 흐르기 시작
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ThisClass::GameTimerCount, 1.0f, true);
	}
}

void APlayingGameMode::EnableAllInput() //모든 플레이어 input 허용
{
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		EHPlayerController->Client_EnableInput();
	}
}


void APlayingGameMode::PlayerLogOut(const AEHPlayerController* ConstExitingEHPlayerController)
{
	AEHPlayerController* ExitingEHPlayerController = const_cast<AEHPlayerController*>(ConstExitingEHPlayerController);
	if(ExitingEHPlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Remove exit player information..."));
	
		int PlayerIndex = EHPlayerControllers.IndexOfByKey(ExitingEHPlayerController);
		
		if (PlayerIndex != INDEX_NONE) EHPlayerControllers.RemoveAt(PlayerIndex);

		//이러면 나간 자리는 초기화 해줘야겠네
		UpdateGameStateNames();
		UpdateGameStateClasses();
		UpdateGameStateExps();
		UpdateGameStateLevels();
		UpdateGameStateHealths();

		/*
		if(이미 죽은 플레이어) NumDeadPlayers--;
		CheckAllPlayerDead();
		*/
	}
}

void APlayingGameMode::SendChatMessage(const FText& Text)
{
	int32 NumberOfPlayers = EHPlayerControllers.Num();

	UE_LOG(LogTemp, Log, TEXT("Send a message to clients"));

	for (int i = 0; i < NumberOfPlayers; i++)
	{
		if (EHPlayerControllers[i])
		{
			EHPlayerControllers[i]->Client_SendChatMessage(Text);
		}
	}
}

void APlayingGameMode::AddPlayerDead()
{
	NumDeadPlayers++;
	CheckAllPlayerDead();
}

void APlayingGameMode::CheckAllPlayerDead()
{
	if(NumDeadPlayers == EHPlayerControllers.Num())
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
		if(EHGameInstance)
		{
			EHGameInstance->bGameClear = false;
			GetWorld()->ServerTravel(GameOverMap, true);
		}
	}
}

void APlayingGameMode::BossDead()
{
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		EHGameInstance->bGameClear = true;
		GetWorld()->ServerTravel(GameOverMap, true);
	}
}


void APlayingGameMode::GameTimerCount()
{
	GameTimer++;

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateHUDGameTimer(GameTimer);
}

void APlayingGameMode::UpdateGameStateHealths()
{
	TArray<float> PlayerMaxHealths;
	TArray<float> PlayerCurrentHealths;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
				{
					PlayerMaxHealths.Add(StatComponent->GetHealth());
					PlayerCurrentHealths.Add(StatComponent->GetMaxHealth());
				}
			}
		}
	}

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateHealths(PlayerMaxHealths, PlayerCurrentHealths);
}

void APlayingGameMode::UpdateGameStateLevels()
{
	TArray<int> PlayerLevels;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerLevels.Add(StatComponent->GetLevel());
			}
		}
	}

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateLevels(PlayerLevels);
}

void APlayingGameMode::UpdateGameStateExps()
{
	TArray<float> PlayerExps;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
			{
				UStatComponent* StatComponent = EHPlayerState->GetStatComponent();
				if(StatComponent)
					PlayerExps.Add(StatComponent->GetExpPercent());
			}
		}
	}

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateExps(PlayerExps);
}

void APlayingGameMode::UpdateGameStateNames()
{
	TArray<FString> PlayerNames;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			PlayerNames.Add(EHPlayerController->PlayerState->GetPlayerName());
		}
	}

	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateNames(PlayerNames);
}

void APlayingGameMode::UpdateGameStateClasses()
{
	TArray<int> PlayerClasses;
	
	for(AEHPlayerController* EHPlayerController : EHPlayerControllers)
	{
		if(EHPlayerController && EHPlayerController->PlayerState)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(EHPlayerController->PlayerState);
			if(EHPlayerState)
				PlayerClasses.Add(EHPlayerState->PlayerClass);
		}
	}
	
	APlayingGameState* PlayingGameState = Cast<APlayingGameState>(GameState);
	PlayingGameState->UpdateGameStateClasses(PlayerClasses);
}

