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

	/*슈터용 스텟*/
	UPROPERTY()
	class AEHShooter *Shooter;
	
	//헤드샷 피해
	float SH_HeadShot = 1.f;
	//장탄 수
	int32 SH_Clips = 40;
	
	/*슈터 직업 증강 레벨*/
	
	//헤드샷
	int32 HU_HeadShotLv = 0;
	//장탄 수
	int32 HU_ClipsLv = 0;
	//수류탄 강화
	int32 HU_EnhnacedGenerate = 0;
	//부스터 로켓 강화
	int32 HU_EnhancedBooster = 0;
};
