// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooter.h"

#include "ShooterCombatComponent.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHShooter::AEHShooter()
{
	CombatComponent = CreateDefaultSubobject<UShooterCombatComponent>(TEXT("Shooter Combat Component"));
	SetMaxPitchAngle(70.f);
	SetMinPitchAngle(-60.f);
}

void AEHShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEHShooter::Shoot()
{
	Super::Shoot();
	if(CombatComponent)
	{
		CombatComponent->Fire();
	}
}

void AEHShooter::Skill()
{
	Super::Skill();
	if(CombatComponent)
	{
		CombatComponent->GrenadeFire();
	}
}

void AEHShooter::BeginPlay()
{
	Super::BeginPlay();
}

void AEHShooter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AEHShooter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AEHPlayerState *NewPlayerState = Cast<AEHPlayerState>(NewController->PlayerState);
	if (NewPlayerState)
	{
		ShooterStatComponent = NewPlayerState->ShooterStatComponent;
		if (ShooterStatComponent)
		{
			UE_LOG(LogClass, Warning, TEXT("EHShooter: SUCCESS load ShooterStatComponent"));
			CombatComponent->SetFireRate(ShooterStatComponent->CalFireRate());
			ShooterStatComponent->Shooter = this;
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("EHShooter: FAILED load ShooterStatComponent"));
		}
	}
}
