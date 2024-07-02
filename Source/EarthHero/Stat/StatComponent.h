#pragma once

#include "EarthHero/EHGameInstance.h"
#include "StatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//데미지 처리 함수
	float DamageTaken(float InDamage, TSubclassOf<UDamageType> DamageTypeClass, const FHitResult & HitInfo, AController *Instigator, class AEHCharacter *DamageCausor);

	//스텟 반환 함수
	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float HealthRegeneration() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetNormalDamage() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetAttackSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetSkillDamage() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetMaxExp() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetExp() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetExpPercent() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetSkillCoolTime() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetDashCoolTime() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetDefensePower() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetIncreasedExpGain() const;

	UFUNCTION(BlueprintPure, Category = "Stat")
	float GetJumpPower() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//HeroStat 변수
	UPROPERTY(Replicated)
	FStatStructure HeroStat;
	
	UPROPERTY(Replicated)
	FStatStructure BaseHeroStat;

private:
	
	//초기화 작업을 위한 DataTable과 함수
	UFUNCTION(Server, Reliable, WithValidation)
	void InitializeStatData(FName HeroName);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
