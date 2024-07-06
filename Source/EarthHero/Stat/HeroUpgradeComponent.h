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
	
	// 업그레이드 배열 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TArray<FHeroUpgradeStructure> HeroUpgrades;

	UFUNCTION()
	void PushRandomHeroUpgrade();

protected:
	virtual void BeginPlay() override;

private:
	// 랜덤 업그레이드를 저장할 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	TArray<FHeroUpgradeStructure> RandomUpgrades;
		
};
