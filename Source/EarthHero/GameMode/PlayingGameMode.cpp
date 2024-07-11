// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayingGameMode.h"

#include <string>

#include "EarthHero/GameSession/PlayingGameSession.h"
#include "EarthHero/Player/EHPlayerController.h"


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
				if(EHPlayerControllers.Num() == PlayingGameSession->GetNumPlayersInSession())
					InitLevelSetting();
			}
		}
	}
}

void APlayingGameMode::InitLevelSetting()
{
	UE_LOG(LogTemp, Log, TEXT("InitLevelSetting()"));
	for(int i = 0; i < EHPlayerControllers.Num(); i++)
	{
		//여기서 플레이어 시작위치 정해줘도 스폰에는 문제가 없는데... 보이지는 않지만 폰이 2개 생성되는 것 같음
		UE_LOG(LogTemp, Log, TEXT("InitLevelSetting()2"));
		//AActor* TargetPlayerStart = FindPlayerStart(EHPlayerControllers[i], FString::FromInt(i));
		UE_LOG(LogTemp, Log, TEXT("InitLevelSetting()3"));
		//RestartPlayerAtPlayerStart(EHPlayerControllers[i], TargetPlayerStart);
	}
	
	//플레이어 스테이트에서 플레이어 이름, 체력정보 취합해서 게임 스테이트를 통해 전파
	//모두의 움직임 풀어주고
	//게임 시간초 시작
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