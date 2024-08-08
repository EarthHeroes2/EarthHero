// Fill out your copyright notice in the Description page of Project Settings.


#include "EHArcher.h"

#include "ArcherCombatComponent.h"
#include "EarthHero/PlayerState/EHPlayerState.h"
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

void AEHArcher::Initialize()
{
	Super::Initialize();
	
	if (IsLocallyControlled())
	{
		WeaponMesh->AttachToComponent(FirstPersonHand, FAttachmentTransformRules::KeepRelativeTransform, FName("FPS_LeftHand"));
	}
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
			UE_LOG(LogClass, Warning, TEXT("EHArcher: FAILED load ArcherStatComponent"));
		}
	}
	
	CombatComponent->SetArcher(this);

	Client_CameraShake();
}

void AEHArcher::Client_CameraShake_Implementation()
{
	CameraShake();
}

void AEHArcher::CameraShake()
{
	if(GetVelocity().Length() > 0.f)
	{
		float Scale = GetVelocity().Length() / 750.f;

		UWorld* World = GetWorld();
		if(World)
		{
			APlayerController* LocalPlayerController = Cast<APlayerController>(World->GetFirstPlayerController());
			if(LocalPlayerController)
			{
				LocalPlayerController->PlayerCameraManager->StartCameraShake(Shake, Scale);
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("EHArcher: No PC"));
			}
		}
	}

	GetWorldTimerManager().SetTimer(CameraShakeTimerHandle, this, &ThisClass::CameraShake, 0.3f);
}


