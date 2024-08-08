// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundMeleeMonster.h"

AGroundMeleeMonster::AGroundMeleeMonster() : Super()
{
	AttackRange = 200.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Assets/ParagonMinions/Characters/Minions/Down_Minions/Animations/Melee/Attack_A_Montage.Attack_A_Montage'"));
	if (MontageFinder.Succeeded())
	{
		MyAnimMontage = MontageFinder.Object;
	}
}

void AGroundMeleeMonster::Attack()
{

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if(SkeletalMesh)
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MyAnimMontage);
		}
	}
}