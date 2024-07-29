// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss5Monster.h"

AMidBoss5Monster::AMidBoss5Monster() : Super()
{
	MonsterType = Boss;
	BossNumber = MidBoss5;
	
	AttackRange = 200.f;
}

void AMidBoss5Monster::Attack()
{
	
}
void AMidBoss5Monster::Skill1()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 1 !"));
}
void AMidBoss5Monster::Skill2()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 2 !"));
}