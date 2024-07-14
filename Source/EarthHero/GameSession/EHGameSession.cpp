// Fill out your copyright notice in the Description page of Project Settings.


#include "EHGameSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "EarthHero/Socket/SocketClient.h"



void AEHGameSession::BeginPlay()
{
    Super::BeginPlay();
}

//AGameModeBase::InitGame()에서 불림
bool AEHGameSession::ProcessAutoLogin()
{
    return true;
}

int AEHGameSession::GetNumPlayersInSession()
{
    return NumberOfPlayersInSession;
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
                        case EOnlineSessionState::InProgress:
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

//세션이 시작된 상태일 때 세션을 끝내기 위함
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
                Session->ClearOnEndSessionCompleteDelegate_Handle(EndSessionDelegateHandle); //EndSessionDelegateHandle?
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
            else UE_LOG(LogTemp, Warning, TEXT("Failed to end Session! (From Callback)"));

            DestroySession(); //세션 제거
            
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
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session."));
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
                UE_LOG(LogTemp, Log, TEXT("Destroyed session succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session"));
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
                        &ThisClass::HandleUpdateSessionCompleted));

                // 세션 정보 업데이트
                if (!Session->UpdateSession(SessionName, *SessionSettings, true))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to update session"));
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
                UE_LOG(LogTemp, Log, TEXT("Update session succesfully."));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to update session."));
            }

            Session->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateSessionDelegateHandle);
            UpdateSessionDelegateHandle.Reset();
        }
    }
}