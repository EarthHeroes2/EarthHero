

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

void AEHWarrior::Dash()
{
	Super::Dash();
	if(CombatComponent)
	{
		CombatComponent->JumpAttack();
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
			WarriorStatComponent->Warrior = this;
			CombatComponent->SetWheelWindTick(WarriorStatComponent->WR_WheelWindTick);
			CombatComponent->SetWheelWindDuration(WarriorStatComponent->WR_WheelWindDuration);
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("EHWarrior: FAILED load WarriorStatComponent"));
		}
	}
}

