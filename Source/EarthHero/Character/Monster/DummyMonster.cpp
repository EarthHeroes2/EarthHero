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
	AttackRange = 800.f;

	AISightRadius = 800.f;
	AILoseSightRadius = 1000.f;
	AIFieldOfView = 90.f; //시야각인데 *2가 됨
	AISightAge = 4.f;
	AILastSeenLocation = 1100.f;
}

void ADummyMonster::Attack(FVector SpawnNormalVector)
{
	UE_LOG(LogTemp, Log, TEXT("ADummyMonster::Attack ADummyMonster::Attack"));
	
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