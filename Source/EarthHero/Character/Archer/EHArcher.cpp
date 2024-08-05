// Fill out your copyright notice in the Description page of Project Settings.


#include "EHArcher.h"

#include "ArcherCombatComponent.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHArcher::AEHArcher()
{
	CombatComponent = CreateDefaultSubobject<UArcherCombatComponent>(TEXT("Archer Combat Component"));
	SetMaxPitchAngle(70.f);
	SetMinPitchAngle(-60.f);
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

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AEHArcher::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEHPlayerState* NewPlayerState = Cast<AEHPlayerState>(NewController->PlayerState);
	if(NewPlayerState)
	{
		ArcherStatComponent = NewPlayerState->ArcherStatComponent;
		if(ArcherStatComponent)
		{
			UE_LOG(LogClass, Warning, TEXT("EHArcher: SUCCESS load ArcherStatComponent"));
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("EHWarrior: FAILED load WarriorStatComponent"));
		}
	}
}


