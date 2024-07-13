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

	void SetInGameHUD(class UInGameHUD *ControllerInGameHUD);

	void SetTabHUD(class UTabHUDWidget *ControllerTabHUD);
	
	// 업그레이드 배열 목록
	UPROPERTY(ReplicatedUsing=OnRep_HeroUpgrades, EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	TArray<FHeroUpgradeStructure> HeroUpgrades;

	UFUNCTION(Server, Reliable)
	void PushRandomHeroUpgrade();

	UFUNCTION(Client, Reliable)
	void PushHeroUpgrade(const TArray<FHeroUpgradeStructure> &ServerHeroUpgrades);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_HeroUpgrades();

protected:
	virtual void BeginPlay() override;

private:
	// 랜덤 업그레이드를 저장할 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	TArray<FHeroUpgradeStructure> RandomUpgrades;

	UPROPERTY()
	UInGameHUD *InGameHUD;

	UPROPERTY()
	UTabHUDWidget *TabHUD;
		
};
