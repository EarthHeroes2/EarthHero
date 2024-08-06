// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include <EarthHero/GameSession/LobbyGameSession.h>
#include <EarthHero/GameMode/LobbyGameMode.h>

#include "OnlineSubsystem.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Info/PerkInfomation.h"
#include "EarthHero/PlayerState/LobbyPlayerState.h"
#include "Interfaces/VoiceInterface.h"


ALobbyPlayerController::ALobbyPlayerController()
{
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Lobby.WBP_Lobby_C'"));
		if (LobbyWidgetAsset.Succeeded())
			LobbyWidgetClass = LobbyWidgetAsset.Class;
	}
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		if (!IsRunningDedicatedServer())
		{
			ShowLobbyWidget(); //로비 위젯 생성
			Server_InitSetup(EHGameInstance->PerkInfo); //서버에게 자신이 준비됨을 알림
		}
	}
}

//음성
void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsRunningDedicatedServer())
	{
		InputComponent->BindKey(EKeys::V, IE_Pressed, this, &ALobbyPlayerController::StartVoiceChat);
		InputComponent->BindKey(EKeys::V, IE_Released, this, &ALobbyPlayerController::StopVoiceChat);
	}
}

void ALobbyPlayerController::StartVoiceChat()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineVoicePtr Voice = Subsystem->GetVoiceInterface();
		if (Voice.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Voice ON"));
			Voice->StartNetworkedVoice(0);
		}
	}
}

void ALobbyPlayerController::StopVoiceChat()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineVoicePtr Voice = Subsystem->GetVoiceInterface();
		if (Voice.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Voice OFF"));
			Voice->StopNetworkedVoice(0);
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

void ALobbyPlayerController::DestroyCharacter() const
{
	if(LobbyCharacter) LobbyCharacter->Destroy();
}

void ALobbyPlayerController::SetCharacter(AEHCharacter* SpawnedCharacter)
{
	LobbyCharacter = SpawnedCharacter;
}

void ALobbyPlayerController::SetHost()
{
	bHost = true;
}

void ALobbyPlayerController::UpdateDifficulty(int Difficulty)
{
	if(!bHost) Client_UpdateDifficulty(Difficulty);
}



bool ALobbyPlayerController::PerkInfoVerification(int64 PerkInfo)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		PerkInfomation* PerkInfomations = new PerkInfomation(); //이름 정하기 어려워서...
		if(PerkInfomations != nullptr)
		{
			int Level = LobbyGameMode->GetPlayerLevel(PlayerState->GetUniqueId());
			int Point = Level + 2;
			
			int NumOfPerks = PerkInfomations->NumOfPerks;

			if(Level > 0)
			{
				int64 CheckBit = 1;
				int SelectableRange = Level * 5; //임시
				int Cnt = 0;
				int i;
			
				for(i = 0; i < SelectableRange; i++)
					if(PerkInfo & (CheckBit << i))
						Cnt += PerkInfomations->NeedPoint[i];
			
				for( ; i < NumOfPerks; i++)
					if(PerkInfo & (CheckBit << i))
						Cnt += 100000;

				if(Point >= Cnt) return true;
			}
		}
	}
	return false;
}

//클라이언트가 서버에게 준비됨을 알리며 실행되는 함수
void ALobbyPlayerController::Server_InitSetup_Implementation(int64 PerkInfo)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(LobbyGameMode->GameSession);
		if (LobbyGameSession)
		{	
			bool bAdvertise = LobbyGameSession->GetAdvertiseState();
			int Difficulty = 1;
			
			if(EHGameInstance)
			{
				Difficulty = EHGameInstance->Difficulty;

				if(PerkInfoVerification(PerkInfo))
				{
					UE_LOG(LogTemp, Log, TEXT("Verified Perk Info"));

					//검증된 반영구 업그레이드 정보를 저장
					ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
					if (LobbyPlayerState)
					{
						UE_LOG(LogTemp, Log, TEXT("Save Perk Info at PlayerState"));
						LobbyPlayerState->PerkInfo = PerkInfo;

						//클라이언트에게 방장 유무와 현재 광고 상태를 알림
						if (bHost) Client_HostAssignment(true, bAdvertise, Difficulty); 
						else Client_HostAssignment(false, bAdvertise, Difficulty);
						
						LobbyGameMode->JoinedPlayerInitSetup(this);
					}
				}
				else UE_LOG(LogTemp, Log, TEXT("Strange Perk Info"));
			}
		}
	}
}

//서버에게서 방장 유무를 받음 (Server_InitSetup에서 불리거나 게임 세션에서 새로운 방장 할당 후에 불림)
void ALobbyPlayerController::Client_HostAssignment_Implementation(bool bHostAssignment, bool bAdvertise, int Difficulty)
{
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

//기본으로 워리어 선택 (임시)
void ALobbyPlayerController::Client_SelectDefaultCharacter_Implementation()
{
	if (LobbyWidget)
	{
		LobbyWidget->ChangeSelectedButton(Warrior);
	}
	else UE_LOG(LogTemp, Log, TEXT("invalid LobbyWidget"));
}

void ALobbyPlayerController::Server_ClientReadyButtonClicked_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		if (bHost)
		{
			if (LobbyGameMode->PressGameStartButton()) Client_SendToDebugMessage("Game Start!");
			else  Client_SendToDebugMessage("All players should be ready!");
		}
		else LobbyGameMode->TogglePlayerReady(this);
	}
}

//서버로 부터 이름 리스트를 받음
void ALobbyPlayerController::Client_UpdatePlayerNameList_Implementation(const TArray<FString>& PlayerNameList, const TArray<int>& PlayerLevelList)
{
	UE_LOG(LogTemp, Log, TEXT("Player name list received"));

	if (LobbyWidget)
	{
		LobbyWidget->UpdatePlayerNameList(PlayerNameList, PlayerLevelList);
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
			LobbyGameMode->Kick(PlayerNumber);
		}
	}
}

void ALobbyPlayerController::Server_SetPlayerCharacter_Implementation(int ClassType)
{
	EClassType PlayerClass = static_cast<EClassType>(ClassType);
	
	//서버에서 선택한 클래스 캐릭터 자신의 위치에 생성
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
		LobbyGameMode->UpdateCharacter(this, PlayerClass);

	ALobbyPlayerState* LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
	if (LobbyPlayerState)
		LobbyPlayerState->PlayerClass = PlayerClass;
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
	if(!bHost && LobbyWidget) LobbyWidget->UpdateDifficulty(Difficulty);
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
			if(bSuccess)
			{
				LobbyWidget->Password_Etb->SetText(FText());
				LobbyWidget->Password_Etb->SetHintText(FText::FromString("Success!"));
				LobbyWidget->bPasswordSetting = true;
			}
			else
			{
				LobbyWidget->Password_Etb->SetText(FText());
				LobbyWidget->Password_Etb->SetHintText(FText::FromString("Fail..."));
			}
		}
	}
}



void ALobbyPlayerController::Client_FadeOut_Implementation()
{
	if(EHGameInstance) EHGameInstance->ShowSeamlessLoadingScreen();
	
	if (LobbyWidget)
	{
		GetWorldTimerManager().ClearTimer(LobbyWidget->ReadFriendsListTimerHandle);
		LobbyWidget->RemoveFromParent();
	}
}

void ALobbyPlayerController::Client_SendToDebugMessage_Implementation(const FString& Message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, Message);
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}