#pragma once

#include "StatComponent.h"

#include "StatCalculationLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Components/ActorComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"  // FTimerHandle과 TimerManager를 사용하기 위해 필요
#include "VectorUtil.h"

UStatComponent::UStatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);  // 컴포넌트가 네트워크에서 복제될 수 있도록 설정
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//스텟 초기화(서버->클라이언트)
	InitializeStatData("Hero");

	//TakeDamage를 Delecate 설정
	// if (GetOwner())
	// {
	// 	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatComponent::DamageTaken);
	// }
}

// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//체력 재생 (1 * (1 + 체력 재생 능력치))
	HeroStat.Health = UE::Geometry::VectorUtil::Clamp(HeroStat.Health + 1 * (1 + HeroStat.HealthRegeneration), 0.f, HeroStat.MaxHealth);
}

float UStatComponent::DamageTaken(float InDamage, TSubclassOf<UDamageType> DamageTypeClass, const FHitResult & HitInfo, AController* Instigator, AEHCharacter* DamageCausor)
{
	//데미지 계산
	UStatCalculationLibrary::CalNormalDamage(HeroStat, InDamage);
	
	//FString Message = FString::Printf(TEXT("Health : %f"), HeroStat.Health);
	//GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	if (HeroStat.Health <= 0.f)
	{
		FString Message = FString::Printf(TEXT("Dead"));
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	}

	return InDamage;
}

// GameInstance에 저장된 초기 스텟을 불러와 StatComponent의 Base스텟과 스텟을 초기화하는 함수
void UStatComponent::InitializeStatData_Implementation(FName HeroName)
{
	UEHGameInstance* ABGameInstance = Cast<UEHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == ABGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("No GameInstacnce"));
		return ;
	}

	// 테이블이 존재한다면, 해당 값으로 스텟을 초기화
	if (FStatStructure *TempStat = ABGameInstance->GetStatStructure(HeroName))
	{
		BaseHeroStat = *TempStat;
		BaseHeroStat.Health = BaseHeroStat.MaxHealth;
		HeroStat = BaseHeroStat;
	}
	else
	{
		// 데이터 테이블에 없는 행일 때
		//GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("Row %s data doesn't exist"), *HeroName.ToString());
		UE_LOG(LogClass, Warning, TEXT("Row '%s' data doesn't exist."), *HeroName.ToString());
	}
}

bool UStatComponent::InitializeStatData_Validate(FName HeroName)
{
	// 여기에 서버에서의 유효성 검사를 수행합니다.
	// 예: 호출한 클라이언트의 권한 확인 등.
	return true; // 유효성 검사가 성공하면 true를 반환합니다.
}

void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// HeroStat 속성을 복제 목록에 추가, 모든 클라이언트에게 복사
	DOREPLIFETIME(UStatComponent, HeroStat);
	DOREPLIFETIME(UStatComponent, BaseHeroStat);
}

float UStatComponent::GetHealth() const
{
	return HeroStat.Health;
}

float UStatComponent::GetMaxHealth() const
{
	return HeroStat.MaxHealth;
}

float UStatComponent::GetHealthPercent() const
{
	if (HeroStat.MaxHealth > 0)
    {
        return HeroStat.Health / HeroStat.MaxHealth;
    }
    return 0.0f;
}

float UStatComponent::HealthRegeneration() const
{
	return HeroStat.HealthRegeneration;
}

float UStatComponent::GetMovementSpeed() const
{
	return HeroStat.MovementSpeed;
}

float UStatComponent::GetNormalDamage() const
{
	return HeroStat.NormalDamage;
}

float UStatComponent::GetAttackSpeed() const
{
	return HeroStat.AttackSpeed;
}

float UStatComponent::GetSkillDamage() const
{
	return HeroStat.SkillDamage;
}

float UStatComponent::GetMaxExp() const
{
	return HeroStat.MaxExp;
}

float UStatComponent::GetExp() const
{
	return HeroStat.Exp;
}

float UStatComponent::GetExpPercent() const
{
	if (HeroStat.MaxExp > 0)
	{
		return HeroStat.Exp / HeroStat.MaxExp;
	}
	return 0.0f;
}

float UStatComponent::GetSkillCoolTime() const
{
	return HeroStat.SkillCoolTime;
}

float UStatComponent::GetDashCoolTime() const
{
	return HeroStat.DashCoolTime;
}

float UStatComponent::GetDefensePower() const
{
	return HeroStat.DefensePower;
}

float UStatComponent::GetIncreasedExpGain() const
{
	return HeroStat.IncreasedExpGain;
}

float UStatComponent::GetJumpPower() const
{
	return HeroStat.JumpPower;
}
