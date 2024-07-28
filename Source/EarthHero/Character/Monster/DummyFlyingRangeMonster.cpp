// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyFlyingRangeMonster.h"

ADummyFlyingRangeMonster::ADummyFlyingRangeMonster() : Super()
{
	static ConstructorHelpers::FClassFinder<AActor> TestBulletAsset(
		TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_TestBullet.BP_TestBullet_C'"));
	if (TestBulletAsset.Succeeded()) TestBulletClass = TestBulletAsset.Class;

	AttackRange = 1800.f;
	AISightRadius = 1600.f;
	AILoseSightRadius = 1900.f;
	AIFieldOfView = 110.f;
	AISightAge = 3.f;
	AILastSeenLocation = 2200.f;
}

void ADummyFlyingRangeMonster::Attack(FVector SpawnNormalVector)
{
	UWorld* World = GetWorld();
	if (World != nullptr && TestBulletClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		FVector SpawnLocation = GetActorLocation() + SpawnNormalVector * 150;
		
		World->SpawnActor<AActor>(TestBulletClass, SpawnLocation, SpawnNormalVector.Rotation(), SpawnParams);
	}
}
