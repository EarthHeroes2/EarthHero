// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss1Monster.h"

AMidBoss1Monster::AMidBoss1Monster() : Super()
{
	MonsterType = Boss;
	BossNumber = MidBoss1;
	
	AttackRange = 200.f;
}

void AMidBoss1Monster::Attack()
{
	
}