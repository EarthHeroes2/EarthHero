// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss3Monster.h"

AMidBoss3Monster::AMidBoss3Monster() : Super()
{
	MonsterType = Boss;
	BossNumber = MidBoss3;
	
	AttackRange = 200.f;
}

void AMidBoss3Monster::Attack()
{
	
}
void AMidBoss3Monster::Skill1()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 1 !"));
}
void AMidBoss3Monster::Skill2()
{
	UE_LOG(LogTemp, Error, TEXT("SKILL 2 !"));
}