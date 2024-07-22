// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "WarriorStatComponent.generated.h"

UCLASS()
class EARTHHERO_API UWarriorStatComponent : public UStatComponent
{
	GENERATED_BODY()

public:
	UWarriorStatComponent();

	UPROPERTY()
	class AEHWarrior *Warrior;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnRep_HeroStat() override;
	
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void WarriorDamage(AActor* DamagedActor);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void WarriorWheelWindDamage(AActor* DamagedActor);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void WarriorDashDamage(AActor *DamagedActor);

protected:
	virtual void BeginPlay() override;

private:
	virtual void InitializeStatData_Implementation() override;
	
public:
	/*워리어용 스텟*/
	
	//일반 공격 데미지
	float WR_NormalDamage = 40.f;
	//휠윈드 공격 데미지
	float WR_WheelWindDamage = 10.f;
	//도약 데미지
	float WR_JumpDamage = 0.f;
	float WR_JumpDamageMulti = 20.f;
	//휠윈드 지속 시간
	float WR_WheelWindDuration = 3.f;
	//휠 윈드 틱 간격
	float WR_WheelWindTick = 0.25f;
	//흡혈
	float WR_Drain = 0;
	//도약 거리
	float WR_EnHancedJumpDistance = 1.f;
	//도약 속도
	float WR_EnHancedJumpSpeed = 1.f;

	/*워리어 직업 증강 레벨*/
	
	//버서커
	int32 HU_BerserkerLv = 0;
	//가디언
	int32 HU_GuardianLv = 0;
	//도약
	int32 HU_EnhnacedJumpLv = 0;
	//휠윈드
	int32 HU_WheelWindLv = 0;
	
};
