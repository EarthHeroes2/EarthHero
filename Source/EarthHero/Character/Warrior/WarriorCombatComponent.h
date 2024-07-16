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

	void Whirlwind();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Attack();

	UFUNCTION(Server, Reliable)
	void Server_Whirlwind();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Whirlwind();

private:
	UPROPERTY()
	AEHWarrior* Warrior;

	FTimerHandle WarriorTimerHandle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Animation");
	UAnimMontage* TPS_AttackAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Animation");
	UAnimMontage* FPS_AttackAnimMontage;

public:
	FORCEINLINE void SetWarrior(AEHWarrior* NewWarrior) { Warrior = NewWarrior; }
	
};
