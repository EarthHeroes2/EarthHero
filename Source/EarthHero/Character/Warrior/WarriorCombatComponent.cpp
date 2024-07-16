

#include "WarriorCombatComponent.h"

#include "EHWarrior.h"

UWarriorCombatComponent::UWarriorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWarriorCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWarriorCombatComponent::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Triggered!"));
	Server_Attack();
}

void UWarriorCombatComponent::Server_Attack_Implementation()
{
	//NetMulticast_Attack();
}

void UWarriorCombatComponent::NetMulticast_Attack_Implementation()
{
	// FPS
	if(Warrior->IsLocallyControlled())
	{
		if(FPS_AttackAnimMontage)
		{
			Warrior->GetMesh()->GetAnimInstance()->Montage_Play(FPS_AttackAnimMontage);
		}
	}
	//TPS
	else
	{
		if(TPS_AttackAnimMontage)
		{
			Warrior->GetMesh()->GetAnimInstance()->Montage_Play(TPS_AttackAnimMontage);
		}
	}
}


void UWarriorCombatComponent::Whirlwind()
{
}

void UWarriorCombatComponent::NetMulticast_Whirlwind_Implementation()
{
}

void UWarriorCombatComponent::Server_Whirlwind_Implementation()
{
}


