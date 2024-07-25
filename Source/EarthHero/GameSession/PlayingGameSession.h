// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EHGameSession.h"
#include "PlayingGameSession.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API APlayingGameSession : public AEHGameSession
{
	GENERATED_BODY()

	APlayingGameSession();

	virtual void BeginPlay() override;
	virtual void NotifyLogout(const APlayerController* ExitingPlayer) override;
};
