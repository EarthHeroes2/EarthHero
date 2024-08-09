// Fill out your copyright notice in the Description page of Project Settings.


#include "AirMeleeMonster.h"

AAirMeleeMonster::AAirMeleeMonster() : Super()
{
	AttackRange = 200.f;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/Assets/ParagonMinions/Characters/Minions/Down_Minions/Animations/Melee/Attack_A_Montage.Attack_A_Montage'"));
	if (MontageFinder.Succeeded())
		AttackAnimMontage = MontageFinder.Object;
}

void AAirMeleeMonster::Attack()
{
	Multicast_Attack();

	//임시로 여기에 둠
	//StartLocationSocket = FName("weapon_sword_r");
	//EndLocationSocket = FName("FX_Trail_01_R");

	Radius = 10.f;
	
	InDamage = 7.f;
}