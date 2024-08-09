// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundRangeMonster.h"

AGroundRangeMonster::AGroundRangeMonster() : Super()
{
	AttackRange = 700.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageFinder(TEXT("AnimMontage'/Game/ParagonMinions/Characters/Minions/Down_Minions/Animations/Ranged/Fire_A_2_Montage.Fire_A_2_Montage'"));
	if (MontageFinder.Succeeded())
		AttackAnimMontage = MontageFinder.Object;
	
	static ConstructorHelpers::FClassFinder<AActor> BulletAsset(TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_TestBullet.BP_TestBullet_C'"));
	if (BulletAsset.Succeeded())
		BulletClass = BulletAsset.Class;
}

//음...
void AGroundRangeMonster::Attack(FVector SpawnNormalVector)
{
	//임시
	BulletLocationSocket = "Muzzle_Front_XForward";
	
	Super::Attack(SpawnNormalVector);
}