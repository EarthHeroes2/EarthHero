

#include "EHWarrior.h"

#include "WarriorCombatComponent.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

AEHWarrior::AEHWarrior()
{
	CombatComponent = CreateDefaultSubobject<UWarriorCombatComponent>(TEXT("Warrior Combat Component"));
	SetMaxPitchAngle(70.f);
	SetMinPitchAngle(-60.f);
}

void AEHWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEHWarrior::Shoot()
{
	Super::Shoot();
	if(CombatComponent)
	{
		CombatComponent->Attack();
	}
}

void AEHWarrior::Skill()
{
	Super::Skill();
	if(CombatComponent)
	{
		CombatComponent->ToggleWhirlwind();
	}
}

void AEHWarrior::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEHWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AEHWarrior::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AEHPlayerState* NewPlayerState = Cast<AEHPlayerState>(NewController->PlayerState);
	if(NewPlayerState)
	{
		WarriorStatComponent = NewPlayerState->WarriorStatComponent;
		if (WarriorStatComponent)
		{
			UE_LOG(LogClass, Warning, TEXT("EHWarrior: SUCCESS load WarriorStatComponent"));
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("EHWarrior: FAILED load WarriorStatComponent"));
		}
	}
}

