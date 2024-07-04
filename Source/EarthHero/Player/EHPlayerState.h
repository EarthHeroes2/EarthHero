// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Stat/MechanicStatComponent.h"
#include "EarthHero/Stat/WarriorStatComponent.h"
#include "EarthHero/Stat/ShooterStatComponent.h"
#include "EarthHero/Stat/ArcherStatComponent.h"
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
	
	//승언: 임시 컴포넌트 추가, 각 히어로 별 컴포넌트로 대체 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UWarriorStatComponent* WarriorStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UMechanicStatComponent* MechanicStatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UShooterStatComponent* ShooterStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UArcherStatComponent* ArcherStatComponent;
};