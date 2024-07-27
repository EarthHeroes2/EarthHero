// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Stat/Structure/EffectStructure.h"
#include "GameFramework/Actor.h"
#include "EffectBase.generated.h"

UCLASS()
class EARTHHERO_API AEffectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectBase();

	// Effect를 적용하는 함수
	virtual void ApplyEffect(AActor* InTargetActor, float InEffectValue, float InDuration, bool InbIsStackable = false, bool InbIsPermanent = false, bool InbShouldRefreshDuration = false);

	virtual void UpgradeEffect(float InEffectValue);
	
	static TArray<FEffectStructure*> EffectArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Effect를 리셋하는 함수
	virtual void ResetEffect();

	// Effect가 적용될 타겟 액터
	UPROPERTY()
	AActor* TargetActor;

	// Timer 핸들
	FTimerHandle EffectTimerHandle;

	// 적용된 효과 값
	float AppliedEffectValue;

	// 효과 지속 시간
	float EffectDuration;

	// 중첩 가능 여부
	bool bIsStackable;

	//중첩 수
	int EffectCount;
	
	// 영구 효과 여부
	bool bIsPermanent;

	// 지속 시간 갱신 여부
	bool bShouldRefreshDuration;

	// 갱신 여부
	bool bRefresh = false;

	// 동일한 대상에 적용된 효과를 관리하기 위한 맵 액터 -> 적용 효과 -> 효과 객체 주소
	static TMap<AActor*, TMap<TSubclassOf<AEffectBase>, AEffectBase*>> EffectMap;

	int EffectType;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
