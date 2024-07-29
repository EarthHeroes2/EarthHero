// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss1Monster.h"

AMidBoss1Monster::AMidBoss1Monster() : Super()
{
	UE_LOG(LogTemp, Warning, TEXT("CHILD"));
	
	MonsterType = Boss;
	BossNumber = MidBoss1;
	
	AttackRange = 200.f;
}

void AMidBoss1Monster::Attack()
{
	
}
void AMidBoss1Monster::Skill1()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 1 !"));
}
void AMidBoss1Monster::Skill2()
{
	
}