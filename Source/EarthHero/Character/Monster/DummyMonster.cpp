// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyMonster.h"

void ADummyMonster::BeginPlay()
{
	Super::BeginPlay();
}

/*
void ADummyMonster::CheckPlayer()
{
	ATestAIController* AIController = Cast<ATestAIController>(GetController());
	if (AIController && !AIController->bMoveToPlayer)
	{
		//임시로 0번 플레이어 지정
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			FVector PlayerLocation = PlayerPawn->GetActorLocation();
			FVector MonsterLocation = GetActorLocation();
			float DistanceToPlayer = FVector::Dist(PlayerLocation, MonsterLocation);
			
			if (DistanceToPlayer < 200.0f) // 감지 거리
			{
				AIController->UpdatePlayerInfo(PlayerPawn);
				AIController->bMoveToPlayer = true;
			}
			else AIController->bMoveToPlayer = false;
		}
	}
}*/