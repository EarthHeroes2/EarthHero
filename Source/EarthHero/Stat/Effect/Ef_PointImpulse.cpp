// Fill out your copyright notice in the Description page of Project Settings.


#include "Ef_PointImpulse.h"
#include "EarthHero/Character/Monster/MonsterBase.h"

// Sets default values
AEf_PointImpulse::AEf_PointImpulse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// RootComponent를 설정합니다.
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
}

// Called when the game starts or when spawned
void AEf_PointImpulse::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEf_PointImpulse::StartPointImpulse()
{
	if (GetNetMode() == NM_ListenServer)
	{
		//UE_LOG(LogClass, Warning, TEXT("ListenServer activatied"));
		for (auto HitActor : HitActors)
		{
			if (AMonsterBase *Monster = Cast<AMonsterBase>(HitActor))
			{
				MonsterArray.Add(Monster);
			}
		}
		GetWorldTimerManager().SetTimer(ImpulseHandle, this, &ThisClass::PointImpulse, Delay, true);
	}
	else
	{
		//UE_LOG(LogClass, Error, TEXT("ListenServer Not Worked"));
	}
}

void AEf_PointImpulse::PointImpulse()
{
	if (Duration < Time)
	{
		GetWorldTimerManager().ClearTimer(ImpulseHandle);
		Destroy();
	}
	
	for (auto Monster : MonsterArray)
	{
		FVector Direction = GetActorLocation() - Monster->GetActorLocation();
		Direction.Normalize();
		Monster->LaunchCharacter(Direction * Strength, true, true);
		//UE_LOG(LogClass, Warning, TEXT("PointImpulse, Time = %f"), Time);
	}
	Time += Delay;
}
