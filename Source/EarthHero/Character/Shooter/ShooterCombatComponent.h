// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCombatComponent.generated.h"


class AEHShooter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UShooterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShooterCombatComponent();

	void Fire();
	
protected:
	UFUNCTION(Server, Reliable)
	void Server_Fire(FVector TraceStartVector, FVector TraceEndVector, FVector MuzzleVector);
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_Fire(FVector HitLocation, FVector MuzzleVector);

	virtual void BeginPlay() override;
	
	void ResetFire();

private:
	UPROPERTY()
	AEHShooter* Shooter;

	FTimerHandle ShootTimerHandle;
	bool bCanFire = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Fire")
	float FireRate = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Particle")
	UParticleSystem* BulletHitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "AnimMontage")
	UAnimMontage* FPS_FireMontage;
	
	
public:
	FORCEINLINE void SetShooter(AEHShooter* NewShooter) { Shooter = NewShooter; }
	
	FORCEINLINE void SetFireRate(float NewFireRate) { FireRate = NewFireRate; }
};
