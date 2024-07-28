// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "GroundMonster.generated.h"

class UNavigationInvokerComponent;
/**
 * 
 */
UCLASS()
class EARTHHERO_API AGroundMonster : public AMonsterBase
{
	GENERATED_BODY()

	UNavigationInvokerComponent* NavigationInvoker;
	
protected:
	AGroundMonster();
	
};
