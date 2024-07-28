// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"

#include "EarthHero/Stat/Monster/MonsterStatComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MonsterStatComponent = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStatComponent"));
	
	MonsterStatComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//자식에서 구현해주세요 (근거리용)
void AMonsterBase::Attack()
{
}

//자식에서 구현해주세요 (원거리용)
void AMonsterBase::Attack(FVector SpawnNormalVector)
{
}

