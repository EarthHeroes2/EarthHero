// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"

#include "MyAIController.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MonsterStatComponent = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStatComponent"));
	
	MonsterStatComponent->SetIsReplicated(true);

	//테스트용 AIController 추가
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
	AIControllerClass = AMyAIController::StaticClass();
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

