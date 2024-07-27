// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyFlyingMonster.h"

#include "GameFramework/FloatingPawnMovement.h"

ADummyFlyingMonster::ADummyFlyingMonster()
{
	static ConstructorHelpers::FClassFinder<AActor> TestBulletAsset(TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_TestBullet.BP_TestBullet_C'"));
	if (TestBulletAsset.Succeeded()) TestBulletClass = TestBulletAsset.Class;
	
	AttackRange = 2000.f;
	AISightRadius = 2000.f;
	AILoseSightRadius = 2300.f;
	AIFieldOfView = 110.f;
	AISightAge = 3.f;
	AILastSeenLocation = 2500.f;
}

void ADummyFlyingMonster::Attack(FVector SpawnNormalVector)
{
	UWorld* World = GetWorld();
	if (World != nullptr && TestBulletClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		FVector SpawnLocation = GetActorLocation() + SpawnNormalVector * 50;
		
		World->SpawnActor<AActor>(TestBulletClass, SpawnLocation, SpawnNormalVector.Rotation(), SpawnParams);
	}
}