// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss6Monster.h"

AMidBoss6Monster::AMidBoss6Monster() : Super()
{
	MonsterType = Boss;
	BossNumber = MidBoss6;
	
	AttackRange = 200.f;
}

void AMidBoss6Monster::Attack()
{
	
}
void AMidBoss6Monster::Skill1()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 1 !"));
}
void AMidBoss6Monster::Skill2()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 2 !"));
}