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
	StartLocationSocket = FName("RightMeleeStart");
	EndLocationSocket = FName("RigthMeleeEnd");
	
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
	//[스킬 1] 일정 범위 내 모든 캐릭터들이 3초동안 자신의 주변 좁은 범위만 볼 수 있게 함.
	//(쿨타임 : 19초)
}

void AHellGolem::Skill2()
{
	Super::Skill2();
}

void AHellGolem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
