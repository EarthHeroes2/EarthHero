// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EHArcher.generated.h"

UCLASS()
class EARTHHERO_API AEHArcher : public AEHCharacter
{
	GENERATED_BODY()

	friend class UArcherCombatComponent;
	friend class UArcherStatComponent;

public:
	AEHArcher();
	virtual void Tick(float DeltaTime) override;
	virtual void Shoot() override;
	virtual void Skill() override;
	virtual void Dash() override;

protected:
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(Client, Reliable)
	void Client_CameraShake();

	void CameraShake();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> Shake;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UArcherCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UArcherStatComponent* ArcherStatComponent;

	UPROPERTY()
	AController* PlayerController;

public:
	FORCEINLINE UArcherCombatComponent* GetCombatComponent() { return CombatComponent; }

private:
	FTimerHandle CameraShakeTimerHandle;

};
