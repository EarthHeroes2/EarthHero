// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/Monster/GroundMonster.h"
#include "HellGolem.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AHellGolem : public AGroundMonster
{
	GENERATED_BODY()

	AHellGolem();
	virtual void Attack() override;
	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayAttackMontage();
	
	virtual void Skill1() override;
	virtual void Skill2() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials settings")
	UMaterialParameterCollection* MyMaterialParameterCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage *MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	bool CanMeleeAttack;

protected:
	virtual void Tick(float DeltaSeconds) override;
private:
	void DoMeleeTrace();
	void CheckAttackedEnemy(FHitResult HitResult);

	UPROPERTY()
	TArray<AActor*> AttackedEnemy;

	float MeleeDamage = 80.f;
	
};
