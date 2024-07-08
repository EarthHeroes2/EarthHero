// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Stat/MechanicStatComponent.h"
#include "EarthHero/Stat/WarriorStatComponent.h"
#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/ArcherStatComponent.h"
#include "EarthHero/Enum/Enums.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWarriorStatComponent* WarriorStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMechanicStatComponent* MechanicStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UShooterStatComponent* ShooterStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UArcherStatComponent* ArcherStatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UHeroUpgradeComponent* HeroUpgradeComponent;

	UFUNCTION(BlueprintCallable, Category = "Component")
	UStatComponent *GetStatComponent();

	UPROPERTY(Replicated)
	int PlayerClass;

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

	UPROPERTY(Replicated)
	bool IsCopyPropertiesEnd = false;
};