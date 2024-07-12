// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameMode.h"
#include "EarthHero/GameSession/PlayingGameSession.h"
#include "EarthHero/GameState/PlayingGameState.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "EarthHero/Player/EHPlayerState.h"


void APlayingGameMode::BeginPlay()
{
	Super::BeginPlay();
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
	
	APlayingGameSession* PlayingGameSession = Cast<APlayingGameSession>(GameSession);
	if (PlayingGameSession)
	{
		//모든 플레이어가 준비 완료되었다면
		if(NumPlayerControllerReady == PlayingGameSession->GetNumPlayersInSession())
		{
			UpdateGameStateNames();
			UpdateGameStateClasses();
	 
			//모두의 움직임 풀어주고 //모두가 동시에 시작하는 편이 좋긴하지만... 2순위
			EnableAllInput();

			//게임 시간초 시작
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &ThisClass::GameTimerCount, 1.0f, true);
		}
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


		//플레이어 스테이트에서 플레이어 이름, 체력정보 취합해서 게임 스테이트를 통해 전파

		//나간 플레이어는 죽은 것으로 처리 -> 모든 플레이어가 죽었는 지 확인
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

