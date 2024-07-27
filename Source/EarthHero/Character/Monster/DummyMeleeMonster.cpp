// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyMeleeMonster.h"

ADummyMeleeMonster::ADummyMeleeMonster()
{
	AttackType = Melee;
	AttackRange = 200.f;
	AISightRadius = 500.f;
	AILoseSightRadius = 700.f;
	AIFieldOfView = 90.f;
	AISightAge = 3.f;
	AILastSeenLocation = 900.f;
}

void ADummyMeleeMonster::Attack(FVector SpawnNormalVector)
{
	
}