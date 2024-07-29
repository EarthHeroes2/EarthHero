// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss4Monster.h"

AMidBoss4Monster::AMidBoss4Monster() : Super()
{
	MonsterType = Boss;
	BossNumber = MidBoss4;
	
	AttackRange = 200.f;
}

void AMidBoss4Monster::Attack()
{
	
}
void AMidBoss4Monster::Skill1()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 1 !"));
}
void AMidBoss4Monster::Skill2()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 2 !"));
}