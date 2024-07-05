// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EHShooter.generated.h"


UCLASS()
class EARTHHERO_API AEHShooter : public AEHCharacter
{
	GENERATED_BODY()

	friend class UShooterCombatComponent;
	friend class UShooterStatComponent;
	
public:
	AEHShooter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void Shoot() override;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UShooterCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UShooterStatComponent* ShooterStatComponent;

	UPROPERTY()
	AController *PlayerController;
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxPitchAngle;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinPitchAngle;

	FTimerHandle ShootTimerHandle;
	bool bCanFire = true;

};
