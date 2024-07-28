// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyFlyingMeleeMonster.h"

ADummyFlyingMeleeMonster::ADummyFlyingMeleeMonster()
{
	AttackRange = 200.f;
	AISightRadius = 1600.f;
	AILoseSightRadius = 1900.f;
	AIFieldOfView = 110.f;
	AISightAge = 3.f;
	AILastSeenLocation = 2200.f;
}

void ADummyFlyingMeleeMonster::Attack(FVector SpawnNormalVector)
{
	
}