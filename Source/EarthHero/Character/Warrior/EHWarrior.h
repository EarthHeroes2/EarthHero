// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EHWarrior.generated.h"

UCLASS()
class EARTHHERO_API AEHWarrior : public AEHCharacter
{
	GENERATED_BODY()

	friend class UWarriorCombatComponent;
	friend class UWarriorStatComponent;

public:
	AEHWarrior();
	virtual void Tick(float DeltaTime) override;
	virtual void Shoot() override;
	virtual void Skill() override;
	virtual void Dash() override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UWarriorCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UWarriorStatComponent* WarriorStatComponent;

	UPROPERTY()
	AController* PlayerController;

public:
	FORCEINLINE UWarriorCombatComponent* GetCombatComponent() { return CombatComponent; }
};
