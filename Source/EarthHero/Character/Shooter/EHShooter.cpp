// Fill out your copyright notice in the Description page of Project Settings.


#include "EHShooter.h"

#include "ShooterCombatComponent.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHShooter::AEHShooter()
{
	MaxPitchAngle = 70.f;
	MinPitchAngle = -60.f;

	CombatComponent = CreateDefaultSubobject<UShooterCombatComponent>(TEXT("Shooter Combat Component"));
	//승언 : ShooterCombatComponent에서 beginPlay에서 슈터 가져오도록 수정함
	//CombatComponent->SetShooter(this);
	
}

void AEHShooter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 위아래 시야각 제한
	if(Controller)
	{
		FRotator NewRotator = Controller->GetControlRotation();
		float NewPitch;
		if(NewRotator.Pitch < 180)
		{
			NewPitch = FMath::Clamp(NewRotator.Pitch, MinPitchAngle, MaxPitchAngle);
		}
		else
		{
			NewPitch = FMath::Clamp(NewRotator.Pitch-360.f, MinPitchAngle, MaxPitchAngle);
		}
		NewRotator.Pitch = NewPitch;
		Controller->SetControlRotation(NewRotator);
	}
}

void AEHShooter::Shoot()
{
	Super::Shoot();
	CombatComponent->Fire();
}

void AEHShooter::Skill()
{
	Super::Skill();
	CombatComponent->GrenadeFire();
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
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("EHShooter: FAILED load ShooterStatComponent"));
		}
	}
}
