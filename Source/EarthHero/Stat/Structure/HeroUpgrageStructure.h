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

	UPROPERTY(EditAnywhere, Category = "HeroUpgrade")
	FText Explanation[3];

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	int32 UpgradeLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	int HeroUpgradeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroUpgrade")
	UTexture2D* UpgradeImage;

	// == 연산자 오버로드
	bool operator==(const FHeroUpgradeStructure& Other) const
	{
		return UpgradeName.EqualTo(Other.UpgradeName) && 
			   Explanation[UpgradeLevel].EqualTo(Other.Explanation[UpgradeLevel]) && 
			   UpgradeLevel == Other.UpgradeLevel && 
			   HeroUpgradeType == Other.HeroUpgradeType;
	}
};
