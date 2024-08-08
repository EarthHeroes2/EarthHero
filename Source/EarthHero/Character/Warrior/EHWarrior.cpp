

#include "EHWarrior.h"

#include "WarriorCombatComponent.h"
#include "EarthHero/PlayerState/EHPlayerState.h"
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

void AEHWarrior::Initialize()
{
	Super::Initialize();
	
	if (IsLocallyControlled())
	{
		WeaponMesh->AttachToComponent(FirstPersonHand, FAttachmentTransformRules::KeepRelativeTransform, FName("FPS_RightHand"));
	}
	else
	{
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("TPS_RightHand"));
	}
}

void AEHWarrior::BeginPlay()
{
	Super::BeginPlay();

	if(CombatComponent)
	{
		CombatComponent->SetWarrior(this);
	}
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

