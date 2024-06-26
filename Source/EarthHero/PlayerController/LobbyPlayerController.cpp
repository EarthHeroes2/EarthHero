// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "GameFramework/PlayerController.h"
#include <EarthHero/EHGameInstance.h>
#include <Kismet/GameplayStatics.h>


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetWorld()->GetGameInstance());

		//�ϴ� �κ� ���� �������� ���� ��û�� ����
		if (EHGameInstance)
		{
			if (EHGameInstance->IsCheckedPrivate)
			{
				UE_LOG(LogTemp, Log, TEXT("Lobby mode request : Private"));

				//ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
				this->Server_ChangeAdvertiseState(false);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Lobby mode request : Public"));
			}
		}
	}
}

//Ŭ���̾�Ʈ�� �������� �˻� + �����̸� advertise ���� ����
void ALobbyPlayerController::Server_ChangeAdvertiseState_Implementation(bool bAdvertise)
{
	//�κ���Ӽ��ǿ��� �̹� �÷��̾� ������ �����ϰ� bHost�� �Ҵ����־���
	if (bHost == true)
	{
		this->Client_HostAssignment();

		//���� ���� ����


	}
}

//�������� Ŭ���̾�Ʈ���� �˸�
void ALobbyPlayerController::Client_HostAssignment_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Host Assignmented!"));

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT("You are host!!!!!!!!!!")));

	//���� ����
}