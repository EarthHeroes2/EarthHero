// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ef_PointImpulse.generated.h"

UCLASS()
class EARTHHERO_API AEf_PointImpulse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEf_PointImpulse();

	UPROPERTY()
	float Strength;

	UPROPERTY()
	float Radius;

	UPROPERTY()
	APawn *Attacker;

	UPROPERTY()
	TSet<AActor*> HitActors;

	UPROPERTY()
	float Duration = 2.f;

	UPROPERTY()
	float Time = 0.f;

	float Delay = 0.02f;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Components")
	class URadialForceComponent* RadialForceComponent;
	
	void PointImpulse();
	
	UPROPERTY()
	TSet<class AMonsterBase*> MonsterArray;
	
	FTimerHandle ImpulseHandle;
	FTimerHandle DelayHandle;
	FTimerHandle SetFalseSimulatePhysicsHandle;

	void SetFalseSimulatePhysics();
	void Impulse() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
