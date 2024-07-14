// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "ShooterStatComponent.generated.h"

UCLASS()
class EARTHHERO_API UShooterStatComponent : public UStatComponent
{
	GENERATED_BODY()

public:
	//일반 공격 데미지
	float SH_NormalDamage = 10.f;
	//수류탄 범위
	float SH_GrenadeRange = 200.f;
	//수류탄 데미지
	float SH_GrenadeDamage = 40.f;
	//수류탄 적중 시 추가데미지
	float SH_AdditionalDamage = 0.15f;

	//로켓 비행 시간
	float SH_FlightTime = 4.f;
	//로켓 비행 이동 속도
	float SH_FLightSpeed = 1.5f;
	//로켓 비행 간 공격 속도(HeroStat.AttackSpeed에 합연산)
	float SH_FlightAttackSpeed = 0.f;
	
	//헤드샷 피해
	float SH_HeadShot = 1.f;
	//장탄 수
	int32 SH_Clips = 40;
	bool SH_ClipsEternal = false;
	
	/*슈터 직업 증강 레벨*/
	
	//헤드샷
	int32 HU_HeadShotLv = 0;
	//장탄 수
	int32 HU_ClipsLv = 0;
	//수류탄 강화
	int32 HU_EnhnacedGenerate = 0;
	//부스터 로켓 강화
	int32 HU_EnhancedBooster = 0;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ShooterDamage(AActor* DamagedActor, const FHitResult & HitInfo, TSubclassOf<UDamageType> DamageTypeClass, AEHCharacter* DamageCusor);

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void ShooterGrenadeDamage(AActor* DamagedActor);

protected:
	virtual void BeginPlay() override;

private:

	virtual void InitializeStatData_Implementation() override;
	
	UPROPERTY()
	class AEHShooter *Shooter;
	
};
