// Fill out your copyright notice in the Description page of Project Settings.


#include "HellGolem.h"

#include "EarthHero/Stat/Monster/MonsterStatComponent.h"

AHellGolem::AHellGolem()
{
	MonsterType = Boss;
	BossNumber = MainBoss;

	// 평타 공격 Range
	AttackRange = 500;
}

//평타 구현
void AHellGolem::Attack()
{
	Super::Attack();
	MC_PlayAttackMontage();
}

void AHellGolem::MC_PlayAttackMontage_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("PlayingAttackMontage"));
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    
	if (AnimInstance && AttackAnimMontage)
	{
		// 애니메이션 몽타주를 재생합니다.
		AnimInstance->Montage_Play(AttackAnimMontage);
	}
}

void AHellGolem::Skill1()
{
	Super::Skill1();
}

void AHellGolem::Skill2()
{
	Super::Skill2();
}

void AHellGolem::Tick(float DeltaSeconds)
{
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	StartLocation = SkeletalMesh->GetSocketLocation(FName("RightMeleeStart"));
	EndLocation = SkeletalMesh->GetSocketLocation(FName("RigthMeleeEnd"));
	Super::Tick(DeltaSeconds);
}
