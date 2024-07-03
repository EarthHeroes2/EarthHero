// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include <EarthHero/EHGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include "GameFramework/PlayerState.h"
#include <EarthHero/GameSession/LobbyGameSession.h>
#include <EarthHero/GameMode/LobbyGameMode.h>


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
		//로비 위젯 생성
		ShowLobbyWidget();
		
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
		
		//비밀방 여부 서버에게 알려줌
		if (EHGameInstance)
		{
			if (EHGameInstance->IsCheckedPrivate)
			{
				UE_LOG(LogTemp, Log, TEXT("Lobby mode request private? : true"));
			}
			else UE_LOG(LogTemp, Log, TEXT("Lobby mode request private? : false"));
			Server_InitSetup(EHGameInstance->IsCheckedPrivate);
		}
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
void ALobbyPlayerController::Server_InitSetup_Implementation(bool bAdvertise)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	
	//방장이라면 비공개방 설정도 적용
	if (bHost)
	{
		Client_HostAssignment(true); //클라이언트에게 방장 유무를 알림
		
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
	else Client_HostAssignment(false);

	//게임모드에 플레이어 정보 등록
	if (LobbyGameMode)
	{
		LobbyGameMode->AddPlayerInfo(this);
	}
}

//서버에게서 방장 유무를 받음
//함수이름 이상함
void ALobbyPlayerController::Client_HostAssignment_Implementation(bool bHostAssignment)
{
	bHost = bHostAssignment; //클라이언트도 방장 유무는 알고 있지만, 서버에서 항상 확인해주기

	if (LobbyWidget)
	{
		LobbyWidget->HostAssignment(bHost);
	}
	
	if (bHost)
	{
		UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("You are host!!!!!!!!!!")));
	}
	
}

void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	
	//방장이라면 비공개방 설정도 적용
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

void ALobbyPlayerController::Server_ClientReady_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		//������ ���� ���۹�ư���� ó��
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
		//Ŭ���̾�Ʈ�� ���� �����ư���� ó��
		else
		{
			LobbyGameMode->TogglePlayerReady(this); //�κ� �÷��̾� ��Ʈ�ѷ��� �ѱ����� �޴� ���� �÷��̾� ��Ʈ�ѷ�. ū ���� ��������?
		}
	}
}

//�������� �÷��̾� �̸� ����Ʈ �ް� UI ����
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
	FText ChatText = FText::Format(FText::FromString("[{0}] : {1}"), PlayerName, Text);

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

//�÷��̾� ű (���常 ����)
void ALobbyPlayerController::Server_PlayerKick_Implementation(int PlayerNumber)
{
	if (bHost)
	{
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (LobbyGameMode)
		{
			//�̰� �������� ����ϱ�?
			ALobbyPlayerController* TargetLobbyPlayerController = LobbyGameMode->LobbyPlayerControllerArray[PlayerNumber];
			{
				UE_LOG(LogTemp, Log, TEXT("Player %d ClientTravel"), PlayerNumber);

				TargetLobbyPlayerController->ClientTravel("/Game/Maps/StartupMap", ETravelType::TRAVEL_Absolute);
			}

		}
	}
}


//열거형 리플리케이션이 안되네...
void ALobbyPlayerController::Server_SetPlayerCharacter_Implementation(int ClassType)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		LobbyGameMode->UpdateCharacter(this, static_cast<EClassType>(ClassType));
	}
}






//����->Ŭ�� �޽��� ���� (�ӽ�)
void ALobbyPlayerController::Client_SendToDebugMessage_Implementation(const FString& Message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, Message);
}