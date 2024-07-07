#pragma once

#include "StatComponent.h"

#include "StatCalculationLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Components/ActorComponent.h"

#include "CoreMinimal.h"
#include "HeroUpgradeComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"  // FTimerHandle과 TimerManager를 사용하기 위해 필요
#include "VectorUtil.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EarthHero/HUD/InGameHUD.h"

UStatComponent::UStatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);  // 컴포넌트가 네트워크에서 복제될 수 있도록 설정
}

void UStatComponent::SetInGameHUD(UInGameHUD* ControllerInGameHUD)
{
	if (!ControllerInGameHUD)
	{
		UE_LOG(LogClass, Warning, TEXT("StatComponent: ERROR!! InGameHUD is NULL"));
	}
	InGameHUD = ControllerInGameHUD;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//스텟 초기화(서버->클라이언트)
	if (GetOwner()->HasAuthority())
	{
		InitializeStatData("Hero");
	}
}


void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//체력 재생 (1 * (1 + 체력 재생 능력치))
	HeroStat.Health = UE::Geometry::VectorUtil::Clamp(HeroStat.Health + 1 * (1 + HeroStat.HealthRegeneration), 0.f, HeroStat.MaxHealth);
}


/*데미지 받는 함수
 * float InDamage : 받은 데미지
 * TSubclassOf<UDamageType> DamageTypeClass : 받은 데미지 타입(Stat/DamageType 디렉토리에 정의)
 * const FHitResult & HitInfo : 데미지 받은 액터의 HitResult
 * AController* Instigator : 데미지를 준 액터의 컨트롤러
 * AEHCharacter* DamageCausor : 데미지를 준 AEHCharacter
 * 주의 : 이 함수는 StatComponent 내부에서 호출되도록 설계됨, 외부에서 호출하도록 설계할려면 @이승언 에게 알려주세요
 */
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



/* GameInstance에 저장된 초기 스텟을 불러와 StatComponent의 Base스텟과 스텟을 초기화하는 함수
 * FName HeroName : DataTable에서 가져올 RowName = 현재 Hero로 고정
*/
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
		BaseHeroStat.MaxExp = BaseHeroStat.RequiresExp[BaseHeroStat.Level];
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
	return true;
}


/*경험치 획득 시 외부에서 호출하는 함수 (서버에서 호출해야 함)
 *float ExpMount : 얻은 경험치
*/
void UStatComponent::UpdateExp(float ExpMount)
{
	bool isLevelUp = UStatCalculationLibrary::AddExp(HeroStat, ExpMount);
	UpdateExpUI(GetExpPercent(), HeroStat.Level, isLevelUp);
	if (isLevelUp)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelUp.. PushRandomUpgrade Activate"));
		HeroUpgradeComponent->PushRandomHeroUpgrade();
	}
}


/* 경험치 획득 시  UI를 갱신하는 함수 (소유 클라이언트 호출)
 *float ExpPercent : 경험치 Percentage
 *int32 Level : 레벨
 *bool IsLevelUp : 레벨업을 했으면 true, 아니면 false
 */
void UStatComponent::UpdateExpUI_Implementation(float ExpPercent, int32 Level, bool IsLevelUp)
{
	//UI 갱신
	InGameHUD->ExpBar->SetPercent(ExpPercent);
	if (IsLevelUp)
	{
		//레벨 갱신
		//UE_LOG(LogClass, Warning, TEXT("Level Up!! Level is %d"), Level);
		InGameHUD->Level_Num->SetText(FText::FromString(FString::Printf(TEXT("%d"), Level)));
	}
}


void UStatComponent::SetHeroUpgradeStComp(UHeroUpgradeComponent* NewComponent)
{
	HeroUpgradeComponent = NewComponent;
}

/********************************************
 *스텟 가져오는 함수*/
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
		//UE_LOG(LogClass, Warning, TEXT("ExpPercent is %f"), HeroStat.Exp / HeroStat.MaxExp);
		return HeroStat.Exp / HeroStat.MaxExp;
	}
	return 0.0f;
}

float UStatComponent::GetLevel() const
{
	return HeroStat.Level;
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

/*****************************************/

/*서버에서 클라이언트로 복사할 변수들*/
void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//복제 목록에 추가, 소유 클라이언트에게 복사
	DOREPLIFETIME(UStatComponent, HeroStat);
	DOREPLIFETIME(UStatComponent, BaseHeroStat);
	DOREPLIFETIME(UStatComponent, InGameHUD);
	DOREPLIFETIME(UStatComponent, HeroUpgradeComponent);
}
