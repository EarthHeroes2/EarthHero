// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyMonster.h"

ADummyMonster::ADummyMonster()
{
	static ConstructorHelpers::FClassFinder<AActor> TestBulletAsset(TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_TestBullet.BP_TestBullet_C'"));
	if (TestBulletAsset.Succeeded())
	{
		TestBulletClass = TestBulletAsset.Class;
	}
	
	//임시
	AttackRange = 700;

	AISightRadius = 800.f;
	AILoseSightRadius = 1000.f;
	AIFieldOfView = 90.f; //시야각인데 *2가 됨
	AISightAge = 4.f;
	AILastSeenLocation = 1100.f;
}

void ADummyMonster::Attack()
{
	UWorld* World = GetWorld();
	if (World != nullptr && TestBulletClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		FVector SpawnLocation = GetActorLocation() + FVector(100.0f, 0.0f, 0.0f);
		FRotator SpawnRotation = GetActorRotation();
		
		AActor* SpawnedActor = World->SpawnActor<AActor>(TestBulletClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}