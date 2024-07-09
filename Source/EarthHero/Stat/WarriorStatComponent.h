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

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void WarriorDamage(AActor* DamagedActor,  TSubclassOf<UDamageType> DamageTypeClass);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void WarriorWheelWindDamage(AActor* DamagedActor,  TSubclassOf<UDamageType> DamageTypeClass);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void WarriorDashDamage(AActor *DamagedActor,  TSubclassOf<UDamageType> DamageTypeClass);

protected:
	virtual void BeginPlay() override;

private:

	virtual void InitializeStatData_Implementation() override;

	/*워리어용 스텟*/

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
