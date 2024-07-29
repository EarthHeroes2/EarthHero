// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss2Monster.h"

AMidBoss2Monster::AMidBoss2Monster() : Super()
{
	MonsterType = Boss;
	BossNumber = MidBoss2;
	
	AttackRange = 200.f;
}

void AMidBoss2Monster::Attack()
{
	
}
void AMidBoss2Monster::Skill1()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 1 !"));
}

void AMidBoss2Monster::Skill2(AEHCharacter* TargetCharacter)
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 2 !"));
}