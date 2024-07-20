// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EarthHero/Stat/Structure/StatStructure.h"
#include "MonsterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UMonsterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterStatComponent();

	FStatStructure &GetBaseMonsterStat();

	FStatStructure &GetMonsterStat();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	FStatStructure BaseMonsterStat;

	UPROPERTY(Replicated)
	FStatStructure MonsterStat;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//데미지 처리 함수
	float DamageTaken(float InDamage, TSubclassOf<UDamageType> DamageTypeClass, const FHitResult & HitInfo, AController *Instigator, class AEHCharacter *DamageCausor, bool &IsDead);

	
private:
	//초기화 작업을 위한 DataTable과 함수
	UFUNCTION(Server, Reliable)
	void InitializeStatData();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

		
};
