// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Stat/MechanicStatComponent.h"
#include "EarthHero/Stat/WarriorStatComponent.h"
#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/ArcherStatComponent.h"
#include "EarthHero/Stat/HeroUpgradeComponent.h"
#include "GameFramework/PlayerState.h"
#include "EHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AEHPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEHPlayerState();
	
	virtual void CopyProperties(APlayerState* PlayerState) override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWarriorStatComponent* WarriorStatComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMechanicStatComponent* MechanicStatComponent;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Component")
	UShooterStatComponent* ShooterStatComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UArcherStatComponent* ArcherStatComponent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UHeroUpgradeComponent* HeroUpgradeComponent;

	UFUNCTION(BlueprintCallable, Category = "Component")
	UStatComponent *GetStatComponent();

	UFUNCTION(BlueprintCallable, Category = "Component")
	UHeroUpgradeComponent *GetHeroUpgradeComponent();

	UPROPERTY(Replicated)
	int PlayerClass;

	UPROPERTY()
	bool IsSetStatComponentEnd = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	static void DestroyComponent(UStatComponent *Target);

	void LoadHeroUpgradeDatatable();
	
	FTimerHandle SetStatComponentTimerHandle;

	UFUNCTION()
	void SetStatComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* HeroUpgradeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* WarriorHeroUpgradeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* MechanicHeroUpgradeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* ShooterHeroUpgradeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade", meta = (AllowPrivateAccess = "true"))
	UDataTable* ArcherHeroUpgradeDataTable;

public:
	UPROPERTY(Replicated)
	bool IsCopyPropertiesEnd = false;
	
};