// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
//#include "MultiplayerSessionsSubsystem.h"
#include "LobbyRowWidget.h"
#include "../EHGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnlineSubsystemUtils.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "../Socket/SocketClient.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//옵션 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> OptionsWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Options.WBP_Options_C'"));
	if (OptionsWidgetAsset.Succeeded())
	{
		OptionsWidgetClass = OptionsWidgetAsset.Class;
	}
	
	//친구 초대 row 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyRowWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Lobby_Row.WBP_Lobby_Row_C'"));
	if (LobbyRowWidgetAsset.Succeeded())
	{
		LobbyRowWidgetClass = LobbyRowWidgetAsset.Class;
	}
}
	
void UMainMenuWidget::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	/*
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}*/
}

bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Play_Btn)
	{
		Play_Btn->OnClicked.AddDynamic(this, &ThisClass::Play_BtnClicked);
		ButtonArray.Add(Play_Btn);
	}
	if (Join_Btn)
	{
		Join_Btn->OnClicked.AddDynamic(this, &ThisClass::Join_BtnClicked);
		ButtonArray.Add(Join_Btn);
	}
	if (Options_Btn)
	{
		Options_Btn->OnClicked.AddDynamic(this, &ThisClass::OptionsBtnClicked);
		ButtonArray.Add(Options_Btn);
	}
	
	if (Exit_Btn)
	{
		Exit_Btn->OnClicked.AddDynamic(this, &ThisClass::Exit_BtnClicked);
		ButtonArray.Add(Exit_Btn);
	}

	if(CreateLobbyOK_Btn)
	{
		CreateLobbyOK_Btn->OnClicked.AddDynamic(this, &ThisClass::CreateLobbyOKBtnClicked);
		ButtonArray.Add(CreateLobbyOK_Btn);
	}
	if(CreateLobbyCancle_Btn)
	{
		CreateLobbyCancle_Btn->OnClicked.AddDynamic(this, &ThisClass::CreateLobbyCancleBtnClicked);
		ButtonArray.Add(CreateLobbyCancle_Btn);
	}

	if(LobbyList_Btn)
	{
		LobbyList_Btn->OnClicked.AddDynamic(this, &ThisClass::LobbyListBtnClicked);
		ButtonArray.Add(LobbyList_Btn);
	}
	if(FindLobby_Btn)
	{
		FindLobby_Btn->OnClicked.AddDynamic(this, &ThisClass::FindLobbyBtnClicked);
		ButtonArray.Add(FindLobby_Btn);
	}

	return true;
}

void UMainMenuWidget::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Created session, entering game!"));
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		Play_Btn->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			//MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		Join_Btn->SetIsEnabled(true);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Session Found!"));
	}
}

void UMainMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UMainMenuWidget::OnDestroySession(bool bWasSuccessful)
{
}

void UMainMenuWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMainMenuWidget::Play_BtnClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Create Lobby Clicked"));
	
	if(LobbySetting_Bd)
	{
		if(LobbySetting_Bd->GetVisibility() == ESlateVisibility::Collapsed)
		{
			LobbySetting_Bd->SetVisibility(ESlateVisibility::Visible);
		}
		else
			LobbySetting_Bd->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenuWidget::Join_BtnClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Join Lobby Clicked"));
	LeaveSession("JoinLobby");
}

//임시로 지우고 생성하는 방식으로 만듬
void UMainMenuWidget::OptionsBtnClicked()
{
	if(OptionsWidget)
	{
		OptionsWidget->RemoveFromParent();
		OptionsWidget = nullptr;
	}
	else
	{
		OptionsWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), OptionsWidgetClass));
		OptionsWidget->AddToViewport();
	}
}

void UMainMenuWidget::Exit_BtnClicked()
{
	// Get the current player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Ensure the PlayerController is valid
	if (PlayerController)
	{
		// Call the QuitGame function from the UKismetSystemLibrary
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}


void UMainMenuWidget::CreateLobbyOKBtnClicked()
{
	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());
	if (EHGameInstance)
	{
		//인스턴스에 private 여부 잠시 저장
		EHGameInstance->LobbyName = LobbyName_Etb->GetText().ToString();
		EHGameInstance->IsCheckedPrivate = Private_Cb->IsChecked();
	}
	
	USocketClient* NewSocket = NewObject<USocketClient>();
	if(NewSocket) NewSocket->CreateSocket();

	//LeaveSession("CreateLobby");
}
void UMainMenuWidget::CreateLobbyCancleBtnClicked()
{
	LobbySetting_Bd->SetVisibility(ESlateVisibility::Collapsed);
}
















void UMainMenuWidget::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UMainMenuWidget::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}



void UMainMenuWidget::LobbyListBtnClicked()
{
	if(LobbyList_Bd)
	{
		if(LobbyList_Bd->GetVisibility() == ESlateVisibility::Collapsed)
		{
			LobbyList_Bd->SetVisibility(ESlateVisibility::Visible);

			FindLobbys("FindLobby");
		}
		else
			LobbyList_Bd->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenuWidget::FindLobbyBtnClicked()
{
	FindLobbys("FindLobby");
}

void UMainMenuWidget::UpdateLobbyList(TArray<FOnlineSessionSearchResult> FindLobbyList)
{
	TArray<FString> FindLobbyIdList;
	
	for (FOnlineSessionSearchResult FindLobby : FindLobbyList)
	{
		FString LobbyId = FindLobby.GetSessionIdStr();
		
		FindLobbyIdList.Add(LobbyId);
		
		int LobbyIndex = LobbyIdList.IndexOfByKey(LobbyId);

		//이미 존재하는 정보라면 업데이트만
		if(LobbyIndex != INDEX_NONE)
		{
			LobbyRowList[LobbyIndex]->UpdateLobbyInfo(FindLobby);
		}
		else //아니라면 새로 추가
		{
			ULobbyRowWidget* LobbyRowWidget = Cast<ULobbyRowWidget>(CreateWidget(GetWorld(), LobbyRowWidgetClass));
			if(LobbyRowWidget)
			{
				LobbyIdList.Add(FindLobby.GetSessionIdStr());
				LobbyRowList.Add(LobbyRowWidget);
				
				LobbyRowWidget->MainMenuWidget = this;
				LobbyRowWidget->UpdateLobbyInfo(FindLobby);
				
				LobbyList_Scr->AddChild(LobbyRowWidget);
			}
		}
	}

	//기존 정보를 살펴봄
	for (int i = 0; i < LobbyIdList.Num(); )
	{
		int FindLobbyIndex = FindLobbyIdList.IndexOfByKey(LobbyIdList[i]);

		//새정보에 기존 정보가 존재하지 않으면 삭제
		if(FindLobbyIndex == INDEX_NONE)
		{
			LobbyRowList[i]->RemoveFromParent();
			
			LobbyIdList.RemoveAt(i);
			LobbyRowList.RemoveAt(i);
		}
		else i++;
	}
	
	SetButtonsEnabled(true);
}

//메인메뉴 모든 버튼의 enalbed 설정
void UMainMenuWidget::SetButtonsEnabled(bool bEnabled)
{
	for(UButton* Button : ButtonArray)
	{
		if(Button) Button->SetIsEnabled(bEnabled);
	}
	LobbyList_Scr->SetIsEnabled(bEnabled);
}

void UMainMenuWidget::FindLobbys(FString Reason)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

			Search->QuerySettings.SearchParams.Empty();
			Search->MaxSearchResults = 1000;
			Search->bIsLanQuery = false;

			FindSessionReason = Reason;

			FindSessionsDelegateHandle =
				Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
					this,
					&ThisClass::HandleFindSessionsCompleted,
					Search));

			UE_LOG(LogTemp, Log, TEXT("Finding Lobby"));

			//로비 리스트 버튼 및 새로 고침 버튼 막음
			SetButtonsEnabled(false);

			if (!Session->FindSessions(0, Search))
			{
				UE_LOG(LogTemp, Warning, TEXT("Find lobby failed"));
				SetButtonsEnabled(true);
			}
		}
	}
}

void UMainMenuWidget::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Found lobby : %d"), Search->SearchResults.Num());

            	TArray<FOnlineSessionSearchResult> FindLobbyList;

                bool bIsFind = false;

                for (FOnlineSessionSearchResult SessionInSearchResult : Search->SearchResults)
                {
                    FString GameName;
                    bool bKeyValueFound1 = SessionInSearchResult.Session.SessionSettings.Get("GameName", GameName);

                    FString PortNumber;
                    bool bKeyValueFound2 = SessionInSearchResult.Session.SessionSettings.Get("PortNumber", PortNumber);
                    
                    int32 NumberOfJoinedPlayers;
                    bool bKeyValueFound3 = SessionInSearchResult.Session.SessionSettings.Get("NumberOfJoinedPlayers", NumberOfJoinedPlayers);

                    bool bAdvertise;
                    bool bKeyValueFound4 = SessionInSearchResult.Session.SessionSettings.Get("Advertise", bAdvertise);

                	FString LobbyName;
                	bool bKeyValueFound5 = SessionInSearchResult.Session.SessionSettings.Get("LobbyName", LobbyName);

                    if (bKeyValueFound1 && bKeyValueFound2 && bKeyValueFound3 && bKeyValueFound4 && bKeyValueFound5)
                    {
                    	if (GameName == "EH2" &&
								(
									(FindSessionReason == "JoinLobby" && NumberOfJoinedPlayers > 0 && bAdvertise) ||
									(FindSessionReason == "CreateLobby" && NumberOfJoinedPlayers == 0) ||
									(FindSessionReason == "FindLobby" && bAdvertise) //임시로 공개방만 찾음
								)
							)
                    	{
                    		if(bAdvertise)
                    		{
                    			UE_LOG(LogTemp, Log, TEXT("Valid session : %s, %s, %d, true"), *FindSessionReason, *PortNumber, NumberOfJoinedPlayers);
                    		}
                    		else UE_LOG(LogTemp, Log, TEXT("Valid session : %s, %s, %d, false"), *FindSessionReason, *PortNumber, NumberOfJoinedPlayers);
                    		
                        	if(FindSessionReason == "FindLobby")
                        	{
                        		bIsFind = true;
                        		FindLobbyList.Add(SessionInSearchResult); //찾은 로비를 리스트에 추가
                        	}
	                        else
	                        {
	                        	if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
	                        	{
	                        		SessionToJoin = &SessionInSearchResult;
	                        		if (SessionToJoin)
	                        		{
	                        			bIsFind = true;
	                        			JoinSession();
	                        			break;
	                        		}
	                        	}
	                        }

                        }
                    }
                }
                //들어갈 로비를 찾지 못함
            	if (!bIsFind && GEngine)
            	{
            		if (FindSessionReason == "JoinLobby")
            		{
            			GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("No lobby to participate")));
            		}
            		else if (FindSessionReason == "CreateLobby")
            		{
            			GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Unable to create lobby (server full)")));
            		}
            		else if(FindSessionReason == "FindLobby")
            		{
            			GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("No lobby")));
            		}
            		SetButtonsEnabled(true);
            	}
            	
            	if(FindSessionReason == "FindLobby") UpdateLobbyList(FindLobbyList);
            }
            else
            {
            	SetButtonsEnabled(true);
            	
                UE_LOG(LogTemp, Warning, TEXT("Find lobbys failed."));
                if(GEngine)
                    GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Find lobbys failed!")));
            }

            Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
            FindSessionsDelegateHandle.Reset();
        }
    }
}

void UMainMenuWidget::JoinSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            JoinSessionDelegateHandle =
                Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleJoinSessionCompleted));

            UE_LOG(LogTemp, Log, TEXT("Joining session."));

            if (!Session->JoinSession(0, JoinSessionName, *SessionToJoin))
            {
                UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
            	SetButtonsEnabled(true);
            }
        }
    }
}

void UMainMenuWidget::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (Result == EOnJoinSessionCompleteResult::Success)
            {
                UE_LOG(LogTemp, Log, TEXT("Joined session."));
                if (GEngine)
                {
                    if (!Session->GetResolvedConnectString(JoinSessionName, ConnectString))
                    {
                        UE_LOG(LogTemp, Error, TEXT("Could not get connect string."));
                    	SetButtonsEnabled(true);
                        return;
                    }
                	
                	GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("Connect String : %s"), *ConnectString));

                    FURL DedicatedServerURL(nullptr, *ConnectString, TRAVEL_Absolute);
                    FString DedicatedServerJoinError;
                    auto DedicatedServerJoinStatus = GEngine->Browse(GEngine->GetWorldContextFromWorldChecked(GetWorld()), DedicatedServerURL, DedicatedServerJoinError);
                    if (DedicatedServerJoinStatus == EBrowseReturnVal::Failure)
                    {
                        UE_LOG(LogTemp, Error, TEXT("Failed to browse for dedicated server. Error is: %s"), *DedicatedServerJoinError);
                    	SetButtonsEnabled(true);
                    }
                }
            }
            Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
            JoinSessionDelegateHandle.Reset();
        }
    }
}


//혹시 모를 세션 제거
void UMainMenuWidget::LeaveSession(FString Reason)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			SetButtonsEnabled(false);
			
			DestroySessionCompleteDelegatesHandle =
				Session->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::DestroySessionComplete);

			LeaveSessionReason = Reason;

			UE_LOG(LogTemp, Log, TEXT("Leave session"));

			if (!Session->DestroySession(JoinSessionName))
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to DestroySession : %s"), *JoinSessionName.ToString());
			}
		}
	}
}

//제거 후 로비를 목적에 맞게 찾고 목적을 수행
void UMainMenuWidget::DestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			if(LeaveSessionReason != "JoinSelectedLobby")
			{
				//일단 성공이든 실패든 세션 찾고 들어감
				if (bWasSuccessful)
				{
					FindLobbys(LeaveSessionReason);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session"));
					FindLobbys(LeaveSessionReason);
				}
			}
			else
			{
				JoinSession();
			}

			Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegatesHandle);
			DestroySessionCompleteDelegatesHandle.Reset();
		}
	}
}