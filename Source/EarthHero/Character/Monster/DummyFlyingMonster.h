// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "DummyFlyingMonster.generated.h"

class UFloatingPawnMovement;
/**
 * 
 */
UCLASS()
class EARTHHERO_API ADummyFlyingMonster : public AMonsterBase
{
	GENERATED_BODY()

	ADummyFlyingMonster();
	
	

	virtual void Attack(FVector SpawnNormalVector) override;

	TSubclassOf<AActor> TestBulletClass;
};
