// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EHGameSession.h"
#include "GameOverGameSession.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AGameOverGameSession : public AEHGameSession
{
	GENERATED_BODY()

public:
	void SeamlessTravelSuccessCount();
	
};
