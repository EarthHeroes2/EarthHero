// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyMonster.h"

ADummyMonster::ADummyMonster()
{
	//임시
	AttackRange = 700;

	AISightRadius = 800.f;
	AILoseSightRadius = 1000.f;
	AIFieldOfView = 90.f; //시야각인데 *2가 됨
	AISightAge = 4.f;
	AILastSeenLocation = 1100.f;
}