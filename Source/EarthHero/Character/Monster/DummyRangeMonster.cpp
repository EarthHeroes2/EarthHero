// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyRangeMonster.h"

ADummyRangeMonster::ADummyRangeMonster() : Super()
{
	static ConstructorHelpers::FClassFinder<AActor> TestBulletAsset(TEXT("Actor'/Game/Blueprints/Character/Dummy/BP_TestBullet.BP_TestBullet_C'"));
	if (TestBulletAsset.Succeeded()) TestBulletClass = TestBulletAsset.Class;
	
	AttackRange = 700.f;
}

void ADummyRangeMonster::Attack(FVector SpawnNormalVector)
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