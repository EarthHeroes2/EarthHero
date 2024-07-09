// Fill out your copyright notice in the Description page of Project Settings.


#include "EHGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include <EarthHero/PlayerController/LobbyPlayerController.h>
#include <EarthHero/GameMode/LobbyGameMode.h>

#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Socket/SocketClient.h"

void AEHGameSession::BeginPlay()
{
    Super::BeginPlay();

    if(IsRunningDedicatedServer())
    {
        IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
        if (Subsystem)
        {
            IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
            if (Session.IsValid())
            {
                EOnlineSessionState::Type SessionState = Session->GetSessionState(SessionName);
                UE_LOG(LogTemp, Log, TEXT("Session state on BeginPlay: %d"), static_cast<int>(SessionState));
            }
        }
    }
}

//�������� �α����ϴ� ���� ���� (AGameModeBase::InitGame()���� �Ҹ�)
bool AEHGameSession::ProcessAutoLogin()
{
    return true;
}

//NotifyLogout에서 불림
void AEHGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
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

void AEHGameSession::HandleUnregisterPlayerCompleted(FName EOSSessionName, const TArray<FUniqueNetIdRef>& PlayerIds, bool bWasSuccesful)
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
        if (Session.IsValid())
        {
            if (bWasSuccesful)
            {
                UE_LOG(LogTemp, Log, TEXT("Player unregistered in Lobby!"))
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
void AEHGameSession::NotifyLogout(const APlayerController* ExitingPlayer)
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
void AEHGameSession::EndSession()
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

void AEHGameSession::HandleEndSessionCompleted(FName EOSSessionName, bool bWasSuccessful)
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

void AEHGameSession::DestroySession()
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

void AEHGameSession::HandleDestroySessionCompleted(FName EOSSessionName, bool bWasSuccesful)
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
void AEHGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Warning, TEXT("AEHGameSession - EndPlay!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
    Super::EndPlay(EndPlayReason);
    DestroySession();
}



void AEHGameSession::UpdateNumberOfJoinedPlayers()
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
                        &AEHGameSession::HandleUpdateSessionCompleted));

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

void AEHGameSession::HandleUpdateSessionCompleted(FName EOSSessionName, bool bWasSuccesful)
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