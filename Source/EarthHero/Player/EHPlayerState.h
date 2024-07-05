// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Stat/MechanicStatComponent.h"
#include "EarthHero/Stat/WarriorStatComponent.h"
#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/ArcherStatComponent.h"
#include "EarthHero/Enum/Enums.h"
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

	UFUNCTION(BlueprintCallable, Category = "Component")
	UStatComponent *GetStatComponent();
	
	EClassType PlayerClass;

private:
	static void DestroyComponent(UStatComponent *Target);
};