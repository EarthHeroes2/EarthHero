// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

class AMonsterBase;
class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class EARTHHERO_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:
	ABulletBase();

protected:
	virtual void BeginPlay() override;

	AMonsterBase* MonsterBase;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UProjectileMovementComponent* ProjectileMovementComp;

	float BulletDamage;

};
