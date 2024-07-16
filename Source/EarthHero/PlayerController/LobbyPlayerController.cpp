// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include <EarthHero/GameSession/LobbyGameSession.h>
#include <EarthHero/GameMode/LobbyGameMode.h>
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/PlayerState/LobbyPlayerState.h"


ALobbyPlayerController::ALobbyPlayerController()
{
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Lobby.WBP_Lobby_C'"));
		if (LobbyWidgetAsset.Succeeded())
		{
			LobbyWidgetClass = LobbyWidgetAsset.Class;
		}
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		ShowLobbyWidget(); //로비 위젯 생성
		Server_InitSetup(); //서버에게 자신이 준비됨을 알림
	}
}

void ALobbyPlayerController::ShowLobbyWidget()
{
	if (LobbyWidgetClass)
	{
		LobbyWidget = Cast<ULobbyWidget>(CreateWidget(GetWorld(), LobbyWidgetClass));
		
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
			bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
		}
	}
}

//클라이언트가 서버에게 준비됨을 알리며 실행되는 함수
void ALobbyPlayerController::Server_InitSetup_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(LobbyGameMode->GameSession);
		if (LobbyGameSession)
		{	
			bool bAdvertise = LobbyGameSession->GetAdvertiseState();
			int Difficulty = 1;

			UEHGameInstance* EHGameinstance = Cast<UEHGameInstance>(GetGameInstance());
			if(EHGameinstance) Difficulty = EHGameinstance->Difficulty;
			

			//클라이언트에게 방장 유무와 현재 광고 상태를 알림
			if (bHost) Client_HostAssignment(true, bAdvertise, Difficulty); 
			else Client_HostAssignment(false, bAdvertise, Difficulty);
		}
	}
	
	//게임모드에 플레이어 정보 등록
	if (LobbyGameMode) LobbyGameMode->AddPlayerInfo(this);
}

//서버에게서 방장 유무를 받음 (Server_InitSetup에서 불리거나 게임 세션에서 새로운 방장 할당 후에 불림)
void ALobbyPlayerController::Client_HostAssignment_Implementation(bool bHostAssignment, bool bAdvertise, int Difficulty)
{
	UE_LOG(LogTemp, Error, TEXT("Client_HostAssignment!!!"));
	
	bHost = bHostAssignment; //클라이언트도 방장 유무는 알고 있지만, 서버에서 항상 확인해주기
	
	if (LobbyWidget) LobbyWidget->HostAssignment(bHost, bAdvertise, Difficulty);
	else UE_LOG(LogTemp, Error, TEXT("Client_HostAssignment (Invalid LobbyWidget)"));
	
	if (bHost)
	{
		UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));
	}
	
}

//클라이언트의 로비 advertise 설정 변경 요청을 서버가 받음
void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (bHost)
	{
		if (LobbyGameMode)
		{
			ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(LobbyGameMode->GameSession);
			if (LobbyGameSession)
			{
				if (bAdvertise)
				{
					UE_LOG(LogTemp, Log, TEXT("Change advertise state : on"));
				}
				else UE_LOG(LogTemp, Log, TEXT("Change advertise state : off"));

				LobbyGameSession->ChangeAdvertiseState(bAdvertise);
			}
		}
	}
}

//기본으로 슈터 선택 (임시)
void ALobbyPlayerController::Client_SelectDefaultCharacter_Implementation()
{
	if (LobbyWidget)
	{
		LobbyWidget->ChangeSelectedButton(Shooter);
	}
	else UE_LOG(LogTemp, Log, TEXT("invalid LobbyWidget"));
}

void ALobbyPlayerController::Server_ClientReadyButtonClicked_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		//호스트 용
		if (bHost)
		{
			if (LobbyGameMode->PressGameStartButton())
			{
				Client_SendToDebugMessage("Game Start!");
			}
			else
			{
				Client_SendToDebugMessage("All players should be ready!");
			}
		}
		// 일반 플레이어 용
		else
		{
			LobbyGameMode->TogglePlayerReady(this);
		}
	}
}

//서버로 부터 이름 리스트를 받음
void ALobbyPlayerController::Client_UpdatePlayerNameList_Implementation(const TArray<FString>& PlayerNameList)
{
	UE_LOG(LogTemp, Log, TEXT("Player name list received"));

	if (LobbyWidget)
	{
		LobbyWidget->UpdatePlayerNameList(PlayerNameList);
	}
	else UE_LOG(LogTemp, Log, TEXT("invalid LobbyWidget"));
}

//�������� ���� ���� �迭 �ް� UI ����
void ALobbyPlayerController::Client_UpdateReadyState_Implementation(const TArray<bool>& PlayerReadyStateArray)
{
	UE_LOG(LogTemp, Log, TEXT("Player ready state received"));

	if (LobbyWidget)
	{
		LobbyWidget->UpdateReadyState(PlayerReadyStateArray);
	}
	else UE_LOG(LogTemp, Log, TEXT("invalid LobbyWidget"));
}


void ALobbyPlayerController::Server_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Send a chat message"));

	FText PlayerName = FText::FromString(PlayerState->GetPlayerName());
	FText ChatText = FText::Format(FText::FromString("[{0}]: {1}"), PlayerName, Text);

	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		LobbyGameMode->SendChatMessage(ChatText);
	}
}

void ALobbyPlayerController::Client_SendChatMessage_Implementation(const FText& Text)
{
	UE_LOG(LogTemp, Log, TEXT("Receive a chat message"));

	if (LobbyWidget)
	{
		LobbyWidget->AddChatMessage(Text);
	}
}

//서버에게 kick할 플레이어 번호를 넘겨줌
void ALobbyPlayerController::Server_PlayerKick_Implementation(int PlayerNumber)
{
	if (bHost)
	{
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (LobbyGameMode)
		{
			//킥할 플레이어에게 ClientTravel를 하도록 강요함
			ALobbyPlayerController* TargetLobbyPlayerController = LobbyGameMode->LobbyPlayerControllerArray[PlayerNumber];
			{
				UE_LOG(LogTemp, Log, TEXT("Player %d ClientTravel"), PlayerNumber);

				TargetLobbyPlayerController->ClientTravel("/Game/Maps/StartupMap", ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void ALobbyPlayerController::Server_SetPlayerCharacter_Implementation(int ClassType)
{
	EClassType PlayerClass = static_cast<EClassType>(ClassType);
	
	//서버에서 선택한 클래스 캐릭터 자신의 위치에 생성
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		LobbyGameMode->UpdateCharacter(this, PlayerClass);
	}

	ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
	if (LobbyPlayerState)
	{
		LobbyPlayerState->PlayerClass = PlayerClass;
	}
}

void ALobbyPlayerController::Server_SetDifficulty_Implementation(int Difficulty)
{
	if(bHost)
	{
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (LobbyGameMode)
		{
			LobbyGameMode->UpdateDifficulty(Difficulty);
		}
	}
}

void ALobbyPlayerController::Client_UpdateDifficulty_Implementation(int Difficulty)
{
	if(LobbyWidget)
		LobbyWidget->UpdateDifficulty(Difficulty);
}



void ALobbyPlayerController::Server_UpdateLobbyPassword_Implementation(const FString& Password)
{
	if (bHost)
	{
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (LobbyGameMode)
		{
			ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(LobbyGameMode->GameSession);
			if (LobbyGameSession)
			{	
				Client_UpdateLobbyPasswordResult(LobbyGameSession->UpdateLobbyPassword(Password));
			}
		}
	}
}

void ALobbyPlayerController::Client_UpdateLobbyPasswordResult_Implementation(bool bSuccess)
{
	if (bHost)
	{
		if(LobbyWidget && LobbyWidget->Password_Etb)
		{
			if(bSuccess) LobbyWidget->Password_Etb->SetHintText(FText::FromString("Success!"));
			else LobbyWidget->Password_Etb->SetHintText(FText::FromString("Fail..."));
		}
	}
}








void ALobbyPlayerController::Client_SendToDebugMessage_Implementation(const FString& Message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, Message);
}






void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (LobbyWidget)
	{
		GetWorldTimerManager().ClearTimer(LobbyWidget->ReadFriendsListTimerHandle);
		LobbyWidget->RemoveFromParent();
	}
	Super::EndPlay(EndPlayReason);
}