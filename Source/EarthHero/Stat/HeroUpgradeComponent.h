// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structure/HeroUpgrageStructure.h"
#include "HeroUpgradeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UHeroUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHeroUpgradeComponent();
	
	// 업그레이드 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TArray<FHeroUpgradeStructure> HeroUpgrades;

protected:
	virtual void BeginPlay() override;
		
};
