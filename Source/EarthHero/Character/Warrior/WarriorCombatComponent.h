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

	void JumpAttack();

	void JumpAttackLanded();

	void ToggleWhirlwind();

	UFUNCTION(Server, Reliable)
	void CheckWhirlWindLevel();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void Server_Attack(int32 ComboOfAttack);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Attack(int32 ComboOfAttack);

	void ResetAttack();

	UFUNCTION(Server, Reliable)
	void Server_JumpAttack(FVector LaunchVector);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_JumpAttack();

	UFUNCTION(Server, Reliable)
	void Server_JumpAttackLanded();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_JumpAttackLanded();

	void JumpAttackEnd();

	void ResetJumpAttack();
	
	UFUNCTION(Server, Reliable)
	void Server_SwordHit(FVector CamLocation, FRotator CamRotation);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SwordHit(FHitResult HitResult);
	
	void Whirlwind();
	
	UFUNCTION(Server, Reliable)
	void Server_Whirlwind(FVector CamLocation);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Whirlwind(FHitResult HitResult);

	void ResetWhirlWind();
private:
	UPROPERTY()
	AEHWarrior* Warrior;

	FTimerHandle AttackCooldownTimerHandle;
	FTimerHandle WhirlwindCooldownTimerHandle;
	FTimerHandle JumpAttackCooldownTimerHandle;
	FTimerHandle WhirlwindTimerHandle;
	FTimerHandle JumpAttackEndTimerHandle;

	//Normal Attack
	UPROPERTY()
	bool bCanAttack = true;
	int32 AttackCombo = 0;
	float AttackCooldown = 0.5f;

	//WhirlWind
	UPROPERTY()
	bool bIsWhirlwind = false;
	UPROPERTY()
	bool bCanWhirlwind = true;

	//Jump Attack
	UPROPERTY()
	bool bIsSuperJump = false;
	UPROPERTY()
	bool bCanSuperJump = true;
	
	float TotalWhirlwindDuration = 3.f;
	float CurrentWhirlwindDuration = 0;
	float WhirlwindCooldown = 14.f;
	float WhirlwindTick = 0.25f;
	float JumpAttackCooldown = 9.f;

	float SkillCoolDown = 1.f;
	float DashCoolDown = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* SwordHitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* WhirlwindHitParticle;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Animation");
	TArray<UAnimMontage*> TPS_AttackAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Animation");
	TArray<UAnimMontage*> FPS_AttackAnimMontage;

public:
	
	FORCEINLINE void SetWarrior(AEHWarrior* NewWarrior) { Warrior = NewWarrior; }
	FORCEINLINE bool GetIsWhirlwind() { return bIsWhirlwind; }
	FORCEINLINE bool GetIsSuperJump() { return bIsSuperJump; }
	FORCEINLINE void SetIsSuperJump(bool bSuperJump) { bIsSuperJump = bSuperJump; }
	
	UFUNCTION(NetMulticast, Reliable)
	void SetWheelWindTick(float WR_WheelWindTick);
	UFUNCTION(NetMulticast, Reliable)
	void SetWheelWindDuration(float WR_WheelWindDuration);
	UFUNCTION(NetMulticast, Reliable)
	void SetSkillCoolDown(float NewSkillCoolDown);
	UFUNCTION(NetMulticast, Reliable)
	void SetDashCoolDown(float NewDashCoolDown);
};
