// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WarriorCombatComponent.generated.h"


class AEHWarrior;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UWarriorCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWarriorCombatComponent();

	void Attack();

	void SwordHit();

	void Whirlwind();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Attack();

	void ResetAttack();

	UFUNCTION(Server, Reliable)
	void Server_SwordHit(FVector CamLocation, FRotator CamRotation);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SwordHit(FHitResult HitResult);

	UFUNCTION(Server, Reliable)
	void Server_Whirlwind();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Whirlwind();

private:
	UPROPERTY()
	AEHWarrior* Warrior;

	FTimerHandle WarriorTimerHandle;

	//Normal Attack
	UPROPERTY()
	bool bCanAttack = true;
	int32 AttackCombo = 0;
	float AttackCooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* SwordHitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Animation");
	UAnimMontage* TPS_AttackAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Animation");
	TArray<UAnimMontage*> FPS_AttackAnimMontage;

public:
	
	FORCEINLINE void SetWarrior(AEHWarrior* NewWarrior) { Warrior = NewWarrior; }
	
};
