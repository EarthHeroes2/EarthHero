// Fill out your copyright notice in the Description page of Project Settings.


#include "EHArcher.h"

AEHArcher::AEHArcher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEHArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEHArcher::Shoot()
{
	Super::Shoot();
}

void AEHArcher::Skill()
{
	Super::Skill();
}

void AEHArcher::Dash()
{
	Super::Dash();
}


void AEHArcher::BeginPlay()
{
	Super::BeginPlay();
}

void AEHArcher::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEHArcher::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}


