// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EarthHero/Enum/Enums.h"
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

	UPROPERTY(BlueprintReadWrite, Category="MonsterStat", ReplicatedUsing = "OnRep_BaseMonsterStat")
	FStatStructure BaseMonsterStat;

	UPROPERTY(BlueprintReadWrite, Category="MonsterStat", ReplicatedUsing = "OnRep_MonsterStat")
	FStatStructure MonsterStat;

public:
	
	//초기화 작업을 위한 DataTable과 함수
	UFUNCTION(Server, Reliable)
	void InitializeStatData();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//데미지 처리 함수
	float DamageTaken(float InDamage, TSubclassOf<UDamageType> DamageTypeClass, const FHitResult & HitInfo, AController *Instigator, class AEHCharacter *DamageCausor, bool &IsDead);
	float GiveNormalDamage(AActor* DamagedActor, float Damage);

	UFUNCTION()
	void OnRep_BaseMonsterStat();
	UFUNCTION()
	void OnRep_MonsterStat();
	
	UPROPERTY()
	class AMonsterBase *Monster;

	UFUNCTION(NetMulticast, Reliable)
	void UpdateEffectImage(UTexture2D* EffectImage, int ServerEffectType, float Duration);

	UFUNCTION(NetMulticast, Reliable)
	void SetMonsterHUDVisTrue();

	
private:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void die();
};
