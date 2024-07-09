// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include <EarthHero/GameMode/LobbyGameMode.h>

#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Socket/SocketClient.h"


void ALobbyGameSession::BeginPlay()
{
    Super::BeginPlay();

    //dedicated 서버에서만 실행
    if (IsRunningDedicatedServer() && !bSessionExists)
    {
        UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());

        if(EHGameInstance)
        {
            FString PortNumber;
            UWorld* World = GetWorld();
            
            if (World) PortNumber = FString::FromInt(World->URL.Port);
            
            if (!PortNumber.IsEmpty())
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby port : %s"), *PortNumber);
                EHGameInstance->ServerPortNumber = PortNumber;
                CreateSession(PortNumber);
            }
            else UE_LOG(LogTemp, Error, TEXT("Port number error"));
        }
    }
}

void ALobbyGameSession::CreateSession(FString PortNumber)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            CreateSessionDelegateHandle =
                Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleCreateSessionCompleted));
            
            TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
            SessionSettings->bIsDedicated = true;
            SessionSettings->bIsLANMatch = false;
            SessionSettings->NumPublicConnections = MaxNumberOfPlayersInSession;
            SessionSettings->NumPrivateConnections = 0;
            SessionSettings->bUsesPresence = false; // 무조건 false여야 함
            SessionSettings->bUseLobbiesIfAvailable = false; // 무조건 false여야 함
            SessionSettings->bAllowInvites = true;
            SessionSettings->bAllowJoinInProgress = false;
            SessionSettings->bShouldAdvertise = false; //일단 무조건 public
            
            //SessionSettings->bUsesStats = false; // 업적관련?
            //SessionSettings->bAntiCheatProtected = true; // 지원하나?

            FString LobbyName;
            FParse::Value(FCommandLine::Get(), TEXT("LobbyName="), LobbyName);

            FString Private; 
            FParse::Value(FCommandLine::Get(), TEXT("Private="), Private);
            
            SessionSettings->Set("GameName", FString("EH2"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("PortNumber", PortNumber, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("NumberOfJoinedPlayers", 0, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            SessionSettings->Set("LobbyName", LobbyName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            if(Private == "true") SessionSettings->Set("Advertise", false, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
            else SessionSettings->Set("Advertise", true, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

            UE_LOG(LogTemp, Error, TEXT("ln = %s     p = %s"), *LobbyName, *Private);

            UE_LOG(LogTemp, Log, TEXT("Creating lobby..."));
            
            if (!Session->CreateSession(0, SessionName, *SessionSettings))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
                Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
                CreateSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleCreateSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                bSessionExists = true;
                UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *EOSSessionName.ToString());
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
            
            Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
            CreateSessionDelegateHandle.Reset();
        }
    }
}

//�������� �α����ϴ� ���� ���� (AGameModeBase::InitGame()���� �Ҹ�)
bool ALobbyGameSession::ProcessAutoLogin()
{
    return true;
}

void ALobbyGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
    Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

    if (IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

            if (Session.IsValid())
            {
                NewPlayerPlayerController = NewPlayer;

                RegisterPlayerDelegateHandle =
                    Session->AddOnRegisterPlayersCompleteDelegate_Handle(FOnRegisterPlayersCompleteDelegate::CreateUObject(
                        this,
                        &ThisClass::HandleRegisterPlayerCompleted));

                //���ǿ� �÷��̾� ���
                if (!Session->RegisterPlayer(SessionName, *UniqueId, false))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Register Player!"));
                    Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
                    RegisterPlayerDelegateHandle.Reset();
                }
            }
        }
    }
}

//�÷��̾� ��� ���
//PlayerIds�� �¶��� ���񽺿� ����� ��� �÷��̾� id�� �ǹ�
void ALobbyGameSession::HandleRegisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player registered in Lobby!"));

                NumberOfPlayersInSession++;

                //세션 정보에 참가한 사람 수 업데이트
                UpdateNumberOfJoinedPlayers();

                //첫플레이어 = 방장
                if (NumberOfPlayersInSession == 1)
                {
                    HostPlayerId = PlayerIds[0]; //이거 플레이어 컨트롤러 저장으로 변경 필요
                    UE_LOG(LogTemp, Log, TEXT("Host Assigment..."));

                    //클라이언트에게 방장 권한을 부여
                    HostAssignment(NewPlayerPlayerController);
                }
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to register player! (From Callback)"));

            // Clear and reset delegates
            Session->ClearOnRegisterPlayersCompleteDelegate_Handle(RegisterPlayerDelegateHandle);
            RegisterPlayerDelegateHandle.Reset();
        }
    }
}

//세션 시작
void ALobbyGameSession::StartSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            StartSessionDelegateHandle =
                Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleStartSessionCompleted));

            //���� ���� �õ�
            if (!Session->StartSession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to start lobby!"));
                Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
                StartSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleStartSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Lobby Started!"));

                ChangeMap();
            }
            else UE_LOG(LogTemp, Warning, TEXT("Failed to start lobby!! (From Callback)"));

            Session->ClearOnStartSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
            StartSessionDelegateHandle.Reset();
        }
    }
}

void ALobbyGameSession::ChangeMap()
{
    // 임시
    GetWorld()->ServerTravel(InGameMap, true);
}

//NotifyLogout에서 불림
void ALobbyGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
    Super::UnregisterPlayer(ExitingPlayer);

    if (IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
            if (Session.IsValid())
            {
                // 게임모드의 관리 리스트에서 제거
                ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
                if(LobbyGameMode)
                {
                    UE_LOG(LogTemp, Log, TEXT("Remove exit player information..."));
                    
                    const ALobbyPlayerController* ExitingLobbyPlayerController = Cast<ALobbyPlayerController>(ExitingPlayer);
                    LobbyGameMode->RemovePlayerInfo(ExitingLobbyPlayerController);
                }
                
                // 비정상적으로 플레이어가 떠나면 null일 수 있음
                if (ExitingPlayer->PlayerState)
                {
                    UnregisterPlayerDelegateHandle =
                        Session->AddOnUnregisterPlayersCompleteDelegate_Handle(FOnUnregisterPlayersCompleteDelegate::CreateUObject(
                            this,
                            &ThisClass::HandleUnregisterPlayerCompleted));

                    // ���ǿ��� �÷��̾� ����
                    if (!Session->UnregisterPlayer(SessionName, *ExitingPlayer->PlayerState->GetUniqueId()))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
                        Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
                        UnregisterPlayerDelegateHandle.Reset();
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Unregister Player!"));
                    Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
                    UnregisterPlayerDelegateHandle.Reset();
                }
            }
        }
    }
}

void ALobbyGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player unregistered in Lobby!"));

                // 방장이 나갔다면 새로운 방장을 찾음
                for (const FUniqueNetIdRef& PlayerId : PlayerIds)
                {
                    if (HostPlayerId == PlayerId)
                    {
                        UE_LOG(LogTemp, Log, TEXT("Host has left the lobby"));

                        NewHostFind();

                        break;
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to unregister player! (From Callback)"));
            }
            Session->ClearOnUnregisterPlayersCompleteDelegate_Handle(UnregisterPlayerDelegateHandle);
            UnregisterPlayerDelegateHandle.Reset();
        }
    }
}

//플레이어가 세션에서 떠나면 불림
void ALobbyGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
{
    Super::NotifyLogout(ExitingPlayer); //UnregisterPlayer를 호출함

    if (IsRunningDedicatedServer())
    {
        NumberOfPlayersInSession--;

        //나간 사람 수 세션 정보 업데이트
        UpdateNumberOfJoinedPlayers();
        
        //사람이 아무도 없으면 세션 종료
        if (NumberOfPlayersInSession == 0)
        {
            IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
            if (Subsystem)
            {
                IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
                if (Session.IsValid())
                {
                    EOnlineSessionState::Type SessionState = Session->GetSessionState(SessionName);
                    switch (SessionState)
                    {
                        case EOnlineSessionState::Pending:
                            UE_LOG(LogTemp, Log, TEXT("Session is Pending state."));
                            DestroySession();
                            break;
                        case EOnlineSessionState::Starting:
                            UE_LOG(LogTemp, Log, TEXT("Session is Started state."));
                            EndSession();
                            break;
                        default:
                            UE_LOG(LogTemp, Log, TEXT("Session is another state: %d"), static_cast<int>(SessionState));
                            break;
                    }
                }
            }
        }
    }
}

//세션이 시작된 상태일 때 세션을 끝내기 위함           <-현재 테스트 안해봄
void ALobbyGameSession::EndSession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            EndSessionDelegateHandle =
                Session->AddOnEndSessionCompleteDelegate_Handle(FOnEndSessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleEndSessionCompleted));
            
            if (!Session->EndSession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to end session!"));
                Session->ClearOnEndSessionCompleteDelegate_Handle(StartSessionDelegateHandle);
                EndSessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccessful)
            {
                UE_LOG(LogTemp, Log, TEXT("Session ended!"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to end Session! (From Callback)"));
            }

            Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle);
            EndSessionDelegateHandle.Reset();
        }
    }
}

void ALobbyGameSession::DestroySession()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            DestroySessionDelegateHandle =
                Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(
                    this,
                    &ThisClass::HandleDestroySessionCompleted));
            
            if (!Session->DestroySession(SessionName))
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy lobby."));
                Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
                DestroySessionDelegateHandle.Reset();
            }
        }
    }
}

void ALobbyGameSession::HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                bSessionExists = false;
                UE_LOG(LogTemp, Log, TEXT("Destroyed lobby succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy lobby."));
            }

            Session->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
            DestroySessionDelegateHandle.Reset();
        }
    }
    
    //프로세스 종료
    USocketClient* NewSocket = NewObject<USocketClient>(this);
    if(NewSocket) NewSocket->CreateSocket("DestroyServer");
    FGenericPlatformMisc::RequestExit(false);
}

//이 게임 세션이 레벨에서 제거할 때 호출됨
//일단 놔둬봄...
void ALobbyGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Warning, TEXT("LobbyGameSession - EndPlay!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
    Super::EndPlay(EndPlayReason);
    DestroySession();
}

void ALobbyGameSession::ChangeAdvertiseState(bool bAdvertise)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            //기존 세션 정보 받아오고
            FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
            if (SessionSettings)
            {
                //광고 여부 재설정
                SessionSettings->Set("Advertise", bAdvertise, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

                if (bAdvertise)
                {
                    UE_LOG(LogTemp, Log, TEXT("Change advertise state : on"));
                }
                else UE_LOG(LogTemp, Log, TEXT("Change advertise state : off"));

                UpdateSessionDelegateHandle =
                    Session->AddOnUpdateSessionCompleteDelegate_Handle(FOnUpdateSessionCompleteDelegate::CreateUObject(
                        this,
                        &ALobbyGameSession::HandleUpdateSessionCompleted));

                // 세션 정보 업데이트
                if (!Session->UpdateSession(SessionName, *SessionSettings, true))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to update Lobby"));
                    Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
                    UpdateSessionDelegateHandle.Reset();
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No session settings found for session: %s"), *SessionName.ToString());
            }
        }
    }
}

void ALobbyGameSession::ChangeLobbyName(FString LobbyName)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            //기존 세션 정보 받아오고
            FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
            if (SessionSettings)
            {
                //로비(세션)이름 재설정
                SessionSettings->Set("LobbyName", LobbyName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

                UE_LOG(LogTemp, Log, TEXT("Change lobby name : %s"), *LobbyName);

                UpdateSessionDelegateHandle =
                    Session->AddOnUpdateSessionCompleteDelegate_Handle(FOnUpdateSessionCompleteDelegate::CreateUObject(
                        this,
                        &ALobbyGameSession::HandleUpdateSessionCompleted));

                // 세션 정보 업데이트
                if (!Session->UpdateSession(SessionName, *SessionSettings, true))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to update Lobby"));
                    Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
                    UpdateSessionDelegateHandle.Reset();
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No session settings found for session: %s"), *SessionName.ToString());
            }
        }
    }
}

void ALobbyGameSession::UpdateNumberOfJoinedPlayers()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            //기존 세션 정보 받아오고
            FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
            if (SessionSettings)
            {
                SessionSettings->Set("NumberOfJoinedPlayers", NumberOfPlayersInSession, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
                
                UE_LOG(LogTemp, Log, TEXT("Update Number Of Joined Players : %d"), NumberOfPlayersInSession);

                UpdateSessionDelegateHandle =
                    Session->AddOnUpdateSessionCompleteDelegate_Handle(FOnUpdateSessionCompleteDelegate::CreateUObject(
                        this,
                        &ALobbyGameSession::HandleUpdateSessionCompleted));

                // 세션 정보 업데이트
                if (!Session->UpdateSession(SessionName, *SessionSettings, true))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to update Lobby"));
                    Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
                    UpdateSessionDelegateHandle.Reset();
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No session settings found for session: %s"), *SessionName.ToString());
            }
        }
    }
}

void ALobbyGameSession::HandleUpdateSessionCompleted(FName EOSSessionName, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Update lobby succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to update lobby."));
            }

            Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
            UpdateSessionDelegateHandle.Reset();
        }
    }
}


void ALobbyGameSession::NewHostFind()
{
    ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
    if(LobbyGameMode)
    {
        if(LobbyGameMode->LobbyPlayerControllerArray.Num() > 0)
        {
            APlayerController* NewHostPlayerController = LobbyGameMode->LobbyPlayerControllerArray[0];
            if(NewHostPlayerController)
            {
                ALobbyPlayerController* NewHostLobbyPlayerController = Cast<ALobbyPlayerController>(NewHostPlayerController);

                if (NewHostLobbyPlayerController)
                {
                    APlayerState* PlayerState = NewHostLobbyPlayerController->PlayerState;
                
                    UE_LOG(LogTemp, Log, TEXT("Host Assignment..."));

                    if (PlayerState && PlayerState->GetUniqueId().IsValid())
                    {
                        HostPlayerId = PlayerState->GetUniqueId();
                        HostAssignment(NewHostPlayerController);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to get unique ID"));
                    }
                }
            }
            else UE_LOG(LogTemp, Warning, TEXT("player controller 0 is not vaild"));
        }
        else UE_LOG(LogTemp, Warning, TEXT("no player controller"));
    }
}

//한 클라이언트에게 방장 할당
void ALobbyGameSession::HostAssignment(APlayerController* HostPlayer)
{
    if (HostPlayer)
    {
        ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(HostPlayer);
        if (LobbyPlayerController)
        {
            UE_LOG(LogTemp, Log, TEXT("Host Assignment : %s"), *HostPlayerId->ToString());

            IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
            if (Subsystem)
            {
                IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
                if (Session.IsValid())
                {
                    FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
                    if (SessionSettings)
                    {
                        FOnlineSessionSetting* AdvertiseState = SessionSettings->Settings.Find("Advertise");
                        if(AdvertiseState)
                        {
                            bool bAdvertise = AdvertiseState->Data.ToString().ToBool();
                            if(bAdvertise)
                            {
                                UE_LOG(LogTemp, Log, TEXT("Current Advertise : true"));
                            }
                            else UE_LOG(LogTemp, Log, TEXT("Current Advertise : false"));

                            LobbyPlayerController->bHost = true;
                            LobbyPlayerController->Client_HostAssignment(true, false, bAdvertise); //advertise는 새로운 방장에게 현재 advertise 여부를 알리기 위함
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to get advertise state"));
                    }
                }
            }
            
        }
    }
}