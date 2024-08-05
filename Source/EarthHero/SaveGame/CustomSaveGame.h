// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	int64 PerkInfo;
};
