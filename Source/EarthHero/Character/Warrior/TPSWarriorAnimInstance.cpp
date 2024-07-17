// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWarriorAnimInstance.h"

#include "EHWarrior.h"
#include "WarriorCombatComponent.h"

void UTPSWarriorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UTPSWarriorAnimInstance::AnimNotify_SwordHit()
{
	if(Character)
	{
		AEHWarrior* Warrior = Cast<AEHWarrior>(Character);
		if(Warrior && Warrior->GetCombatComponent())
		{
			UWarriorCombatComponent* CombatComponent = Warrior->GetCombatComponent();
			CombatComponent->SwordHit();
		}
	}
}
