#pragma once

#include "StatComponent.h"

#include "StatCalculationLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Components/ActorComponent.h"

#include "CoreMinimal.h"
#include "HeroUpgradeComponent.h"
//#include "SWarningOrErrorBox.h"  //박정익 : 이거 뭔지는 모르겠는데 fetal error나서 잠시 빼둠
#include "GameFramework/Actor.h"
#include "TimerManager.h"  // FTimerHandle과 TimerManager를 사용하기 위해 필요
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/HUD/InGameHUD.h"
#include "EarthHero/HUD/TabHUDWidget.h"
#include "EarthHero/Player/EHPlayerController.h"
#include "EarthHero/Player/EHPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UStatComponent::SetTabHUD(UTabHUDWidget* ControllerTabHUD)
{
	if (!ControllerTabHUD)
	{
		UE_LOG(LogClass, Warning, TEXT("StatComponent: ERROR!! TabHUD is NULL"));
	}
	TabHUD = ControllerTabHUD;
	OnRep_HeroStat();
}

FStatStructure &UStatComponent::GetHeroStat()
{
	return HeroStat;
}

FStatStructure &UStatComponent::GetBaseHeroStat()
{
	return BaseHeroStat;
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//스텟 초기화(서버->클라이언트)
	//GetWorldTimerManager().SetTimer(InitializeTimerHandle, this, &ThisClass::InitializeStatData, 1.f, true);
	
	InitializeStatData();
	GetWorld()->GetTimerManager().SetTimer(RegenHealthTimerHandle, this, &UStatComponent::RegenHealthFunc, 1, true);
}

void UStatComponent::RegenHealthFunc()
{
	if (HeroStat.Health + HeroStat.HealthRegeneration <= HeroStat.MaxHealth)
	{
		HeroStat.Health = HeroStat.Health + HeroStat.HealthRegeneration;
	}
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
	float resultDamage = UStatCalculationLibrary::CalNormalDamage(HeroStat, InDamage * HeroStat.MoreDamageTaken);
	
	UE_LOG(LogTemp, Warning, TEXT("InDamage : %f   ResultDamage : %f"), InDamage, resultDamage);

	if (resultDamage > 0)
	{
		APlayingGameMode *GameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->UpdateGameStateReceiveDamage();
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("HeroStat.Health : %f"), HeroStat.Health);
	
	//FString Message = FString::Printf(TEXT("Health : %f"), HeroStat.Health);
	//GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	if (HeroStat.Health <= 0.f)
	{
		FString Message = FString::Printf(TEXT("Dead"));
		
		AActor* Actor = GetOwner();
		if(Actor)
		{
			AEHPlayerState* EHPlayerState = Cast<AEHPlayerState>(Actor);
			if(EHPlayerState)
			{
				AActor* Actor2 = EHPlayerState->GetOwner();
				if(Actor2)
				{
					AEHPlayerController* EHPlayerController = Cast<AEHPlayerController>(Actor2);\
					if(EHPlayerController)
					{
						ACharacter* Character = EHPlayerController->GetCharacter();
						if(Character)
						{
							//즉시 행동을 멈추고
							AEHCharacter* EHCharacter = Cast<AEHCharacter>(Character);
							if(EHCharacter)
							{
								UPawnMovementComponent* MovementComponent = EHCharacter->GetMovementComponent();
								if(MovementComponent) MovementComponent->StopMovementImmediately();

								//죽음 처리 시작
								EHPlayerController->Dead();
							}
						}
					}
				}
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, Message);
	}

	return resultDamage;
}



/* GameInstance에 저장된 초기 스텟을 불러와 StatComponent의 Base스텟과 스텟을 초기화하는 함수
 * FName HeroName : DataTable에서 가져올 RowName = 현재 Hero로 고정
*/
void UStatComponent::InitializeStatData_Implementation()
{
	// if (GetOwner() && GetOwner()->GetInstigatorController() && GetOwner()->GetInstigatorController()->PlayerState)
	// {
		//GetOwner->GetWorldTimerManager().ClearTimer(InitializeTimerHandle); // 타이머 해제
		
		UEHGameInstance* ABGameInstance = Cast<UEHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (nullptr == ABGameInstance)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("No GameInstacnce"));
			return ;
		}

		// 테이블이 존재한다면, 해당 값으로 스텟을 초기화
		if (FStatStructure *TempStat = ABGameInstance->GetStatStructure("Hero"))
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
			UE_LOG(LogClass, Warning, TEXT("Row 'Hero' data doesn't exist."));
		}
	//}
}


/*경험치 획득 시 외부에서 호출하는 함수 (서버에서 호출해야 함)
 *float ExpMount : 얻은 경험치
*/
void UStatComponent::UpdateExp(float ExpMount)
{
	bool isLevelUp = UStatCalculationLibrary::AddExp(HeroStat, BaseHeroStat, ExpMount);
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
	InGameHUD->Exp_Num->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), FMath::FloorToInt(ExpPercent * 100))));
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

float UStatComponent::GetKillCount() const
{
	return KillCount;
}

float UStatComponent::GetGivenDamage() const
{
	return GivenDamage;
}

float UStatComponent::GetReceiveDamage() const
{
	return ReceiveDamage;
}

float UStatComponent::GetHeal() const
{
	return Heal;
}

/*****************************************/

/*서버에서 클라이언트로 복사할 변수들*/

void UStatComponent::OnRep_HeroStat()
{
	//TabHUD의 정보 업데이트
	// editor listen server 환경에선 서버는 클라이언트를 안가지고 있기 때문에 업데이트가 안되는 게 맞다.
	if (TabHUD)
	{ 
		TabHUD->SetStatusWidgetValues(
			FText::Format(FText::FromString(TEXT("{0}%")),FText::AsNumber(HeroStat.NormalDamage * 100)),
			FText::Format(FText::FromString(TEXT("{0}%")),FText::AsNumber(HeroStat.AttackSpeed * 100)),
			FText::Format(FText::FromString(TEXT("{0}%")),FText::AsNumber(HeroStat.SkillDamage * 100)),
			FText::Format(FText::FromString(TEXT("-{0}%")),FText::AsNumber(100 - HeroStat.SkillCoolTime * 100)),
			FText::Format(FText::FromString(TEXT("-{0}%")),FText::AsNumber(100 - HeroStat.DashCoolTime * 100)),
			FText::AsNumber(FMath::FloorToInt(HeroStat.Health)),
			FText::AsNumber(HeroStat.DefensePower),
			FText::AsNumber(HeroStat.MaxHealth),
			FText::AsNumber(HeroStat.HealthRegeneration),
			FText::Format(FText::FromString(TEXT("{0}%")),FText::AsNumber(HeroStat.MovementSpeed * 100)));
	}
	OnRep_HeroStat_Server();
}

void UStatComponent::OnRep_HeroStat_Server_Implementation()
{
	if (APlayingGameMode *PlayingGameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode()))
	{
		PlayingGameMode->UpdateGameStateHealths();
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("failed to cast PlayingGameMode"));
	}
}

void UStatComponent::OnRep_BaseHeroStat()
{
	//BaseHeroStat이 변경 -> 영구 업그레이드(퍽즈, 히어로 업그레이드)이므로,
	//HeroStat과 동기화 시켜준다.
	// 단, 현재 체력과 현재 경험치, 경험치, 요구 경험치, 레벨은 제외 한다.
	// 또한 상태 이상 적용 중이라면 해당 능력치가 포험된 계산을 다시 수행한다.
	
	HeroStat.MaxHealth = BaseHeroStat.MaxHealth;
	HeroStat.HealthRegeneration = BaseHeroStat.HealthRegeneration;		//체력 재생
	HeroStat.MovementSpeed = BaseHeroStat.MovementSpeed;				//이동속도
	HeroStat.NormalDamage = BaseHeroStat.NormalDamage; 					//일반 공격 데미지
	HeroStat.AttackSpeed = BaseHeroStat.AttackSpeed;					//공격 속도
	HeroStat.SkillDamage = BaseHeroStat.SkillDamage;					//스킬 데미지
	HeroStat.SkillCoolTime = BaseHeroStat.SkillCoolTime;				//공격 스킬 쿨타임 감소
	HeroStat.DashCoolTime = BaseHeroStat.DashCoolTime;					//이동 스킬 쿨타임 감소
	HeroStat.DefensePower = BaseHeroStat.DefensePower;					//보호막
	HeroStat.IncreasedExpGain = BaseHeroStat.IncreasedExpGain;			//경험치 획득량 증가
	HeroStat.JumpPower = BaseHeroStat.JumpPower;						//점프력
}

void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//복제 목록에 추가, 소유 클라이언트에게 복사
	DOREPLIFETIME(UStatComponent, HeroStat);
	DOREPLIFETIME(UStatComponent, BaseHeroStat);
}
