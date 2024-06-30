// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include <EarthHero/EHGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include <EarthHero/GameSession/LobbyGameSession.h>
#include <EarthHero/GameMode/LobbyGameMode.h>


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());

		//�ϴ� �κ� ���� �������� ���� ��û�� ����
		if (EHGameInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("Lobby mode request private? : %s"), EHGameInstance->IsCheckedPrivate);

			Server_ChangeAdvertiseState(EHGameInstance->IsCheckedPrivate);
		}
	}
}

//Ŭ���̾�Ʈ�� �������� �˻� + �����̸� advertise ���� ����
void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{
	//�κ���Ӽ��ǿ��� �̹� �÷��̾� ������ �����ϰ� bHost�� �Ҵ����־���
	if (bHost)
	{
		Client_HostAssignment();

		//���� ���� ����
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (LobbyGameMode)
		{
			ALobbyGameSession* LobbyGameSession = Cast<ALobbyGameSession>(LobbyGameMode->GameSession);
			if (LobbyGameSession)
			{
				UE_LOG(LogTemp, Log, TEXT("Change advertise state..."));

				LobbyGameSession->ChangeAdvertiseState(bAdvertise);
			}
		}

	}
}

//�������� Ŭ���̾�Ʈ���� �˸�
void ALobbyPlayerController::Client_HostAssignment_Implementation()
{
	bHost = true; //Ŭ���̾�Ʈ���� �˸��� ��ŭ, �������� ȣ��Ʈ Ȯ�� �׻��ϱ�

	UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("You are host!!!!!!!!!!")));
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


//�������� ���� ���� �迭 �ް� UI ����
void ALobbyPlayerController::Client_UpdateReadyState_Implementation(const TArray<bool>& PlayerReadyStateArray)
{
	//ui ���� �Լ�ȣ��
}


//����->Ŭ�� �޽��� ���� (�ӽ�)
void ALobbyPlayerController::Client_SendToDebugMessage_Implementation(const FString& Message)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, Message);
}