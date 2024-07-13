// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverPlayerState.h"

#include "OnlineSubsystem.h"
#include "EarthHero/GameMode/GameOverGameMode.h"
#include "Interfaces/OnlineIdentityInterface.h"

void AGameOverPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if(GetNetMode() != NM_Client)
	{
		
		GetWorldTimerManager().SetTimer(SetStatComponentTimerHandle, this, &ThisClass::TestFunc, 4.5f,false);
	}
}


//copy propertice가 끝나고 데이터를 서버에 저장해야함
void AGameOverPlayerState::TestFunc()
{
	TArray<uint8> Contents;
	Contents.Add(TestVar);
	Contents.Add(11);

	
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();
	if (GameMode)
	{
		AGameOverGameMode* GameOverGameMode = Cast<AGameOverGameMode>(GameMode);
		if(GameOverGameMode)
		{
			IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
			if (Subsystem)
			{
				IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
				if (Identity.IsValid())
				{
					FUniqueNetIdPtr UserId = Identity->GetUniquePlayerId(0); // 0?
					if (UserId.IsValid())
					{
						UE_LOG(LogTemp, Error, TEXT("TestFunc()"));
						GameOverGameMode->SaveClientData(*UserId, "TestFile", Contents);
					}
				}
			}
		}
	}
}