// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Enum/Enums.h"
#include "Engine/DataTable.h"
#include "HeroUpgrageStructure.generated.h"

USTRUCT(BlueprintType)
struct FHeroUpgradeStructure : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	FText UpgradeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	FText Explanation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	int32 UpgradeLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	EHeroUpgrade HeroUpgradeType;
};
