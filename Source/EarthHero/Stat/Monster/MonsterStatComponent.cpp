// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"

#include "Components/WidgetComponent.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/HUD/MonsterStatHUD.h"
#include "EarthHero/Stat/StatCalculationLibrary.h"
#include "EarthHero/Stat/StatComponent.h"
#include "EarthHero/Stat/DamageType/NormalDamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UMonsterStatComponent::UMonsterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);  // 컴포넌트가 네트워크에서 복제될 수 있도록 설정
}

FStatStructure& UMonsterStatComponent::GetBaseMonsterStat()
{
	return BaseMonsterStat;
}

FStatStructure& UMonsterStatComponent::GetMonsterStat()
{
	return MonsterStat;
}


void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMonsterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

float UMonsterStatComponent::DamageTaken(float InDamage, TSubclassOf<UDamageType> DamageTypeClass,
	const FHitResult& HitInfo, AController* Instigator, AEHCharacter* DamageCausor, bool &IsDead)
{
	//데미지 계산
	float resultDamage = UStatCalculationLibrary::CalNormalDamage(MonsterStat, InDamage * MonsterStat.MoreDamageTaken);
	
	//FString Message = FString::Printf(TEXT("Health : %f"), MonsterStat.Health);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Message);
	
	if(Monster)
	{
		AController* Controller = Monster->GetController();
		if(Controller)
		{
			AAIControllerBase* AIControllerBase = Cast<AAIControllerBase>(Controller);
			if(AIControllerBase)
			{
				AIControllerBase->AttackedPlayer(DamageCausor);
			}
		}
	}
	
	if (MonsterStat.Health <= 0.f)
	{
		IsDead = true;

		AActor* Owner = GetOwner();
		if (Owner)
		{
			AMonsterBase* MonsterBase = Cast<AMonsterBase>(Owner);
			if(MonsterBase)
			{
				if(MonsterBase->MonsterType == Boss &&
					(MonsterBase->BossNumber == MidBoss1 ||
					MonsterBase->BossNumber == MidBoss3 ||
					MonsterBase->BossNumber == MidBoss5
					)
				)
				{
					APlayingGameMode* PlayingGameMode = Cast<APlayingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					if (PlayingGameMode)
						PlayingGameMode->PlayerRebirthAfterBossDead();
				}
			}
		}
		
		die();
	}

	return resultDamage;
}

float UMonsterStatComponent::GiveNormalDamage(AActor* DamagedActor, float Damage)
{
	float actualDamage = 0;
	if(AEHCharacter* HitCharacter = Cast<AEHCharacter>(DamagedActor))
	{
		static FHitResult DummyHitResult;
		UE_LOG(LogClass, Warning, TEXT("GivenNormalDamge"));
		actualDamage = HitCharacter->StatComponent->DamageTaken(Damage, UNormalDamageType::StaticClass(), DummyHitResult, nullptr, nullptr);
	}
	return actualDamage;
}

void UMonsterStatComponent::OnRep_BaseMonsterStat()
{
	//BaseHeroStat이 변경 -> 영구 업그레이드(퍽즈, 히어로 업그레이드)이므로,
	//HeroStat과 동기화 시켜준다.
	// 단, 현재 체력과 현재 경험치, 경험치, 요구 경험치, 레벨은 제외 한다.
	// 또한 상태 이상 적용 중이라면 해당 능력치가 포험된 계산을 다시 수행한다.
	
	MonsterStat.MaxHealth = BaseMonsterStat.MaxHealth;
	MonsterStat.HealthRegeneration = BaseMonsterStat.HealthRegeneration;		//체력 재생
	MonsterStat.MovementSpeed = BaseMonsterStat.MovementSpeed;				//이동속도
	MonsterStat.NormalDamage =BaseMonsterStat.NormalDamage; 					//일반 공격 데미지
	MonsterStat.AttackSpeed = BaseMonsterStat.AttackSpeed;					//공격 속도
	MonsterStat.SkillDamage = BaseMonsterStat.SkillDamage;					//스킬 데미지
	MonsterStat.SkillCoolTime = BaseMonsterStat.SkillCoolTime;				//공격 스킬 쿨타임 감소
	MonsterStat.DashCoolTime = BaseMonsterStat.DashCoolTime;					//이동 스킬 쿨타임 감소
	MonsterStat.DefensePower = BaseMonsterStat.DefensePower;					//보호막
	MonsterStat.IncreasedExpGain = BaseMonsterStat.IncreasedExpGain;			//경험치 획득량 증가
	MonsterStat.JumpPower =BaseMonsterStat.JumpPower;						//점프력
}

void UMonsterStatComponent::OnRep_MonsterStat()
{
	if (Monster)
	{
		SetMonsterHUDVisTrue();
		if (UMonsterStatHUD *MonsterStatHUD = Cast<UMonsterStatHUD>(Monster->MonsterStatHUDComponent->GetWidget()))
		{
			//UE_LOG(LogClass, Warning, TEXT("MonsterStatComponent : UpdateHealth"));
			MonsterStatHUD->UpdateHealth(MonsterStat.Health / MonsterStat.MaxHealth);
		}
		else
		{
			//UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : UpdateHealth failed"));
		}
	}
}


/* GameInstance에 저장된 초기 스텟을 불러와 StatComponent의 Base스텟과 스텟을 초기화하는 함수
 * FName HeroName : DataTable에서 가져올 RowName = 현재 Hero로 고정
*/
void UMonsterStatComponent::InitializeStatData_Implementation()
{
	UEHGameInstance* ABGameInstance = Cast<UEHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == ABGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("No GameInstacnce"));
		return ;
	}
	
	// 테이블이 존재한다면, 해당 값으로 스텟을 초기화
	FStatStructure *TempStat = nullptr;
	switch (Monster->MonsterType)
	{
		case Boss :
			switch (Monster->BossNumber)
			{
				case MainBoss :
					TempStat =  ABGameInstance->GetStatStructure("MainBoss");
					UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MainBoss"));
					break;
				case MidBoss1 :
					TempStat =  ABGameInstance->GetStatStructure("MidBoss1");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MidBoss1"));
					break;
				case MidBoss2 :
					TempStat =  ABGameInstance->GetStatStructure("MidBoss2");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MidBoss2"));
					break;
				case MidBoss3 :
					TempStat =  ABGameInstance->GetStatStructure("MidBoss3");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MidBoss3"));
					break;
				case MidBoss4 :
					TempStat =  ABGameInstance->GetStatStructure("MidBoss4");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MidBoss4"));
					break;
				case MidBoss5 :
					TempStat =  ABGameInstance->GetStatStructure("MidBoss5");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MidBoss5"));
					break;
				case MidBoss6 :
					TempStat =  ABGameInstance->GetStatStructure("MidBoss6");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : MidBoss6"));
					break;
				case NotBoss :
					TempStat =  ABGameInstance->GetStatStructure("NormalMonster");
				UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : NotBoss"));
					break;
				default:
					UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : failed"));
			}
			break;
		case Normal :
			TempStat =  ABGameInstance->GetStatStructure("NormalMonster");
			UE_LOG(LogClass, Error, TEXT("MonsterStatComponent : NormalMonster"));
			break;
	}
	
	BaseMonsterStat = *TempStat;
	BaseMonsterStat.Health = BaseMonsterStat.MaxHealth;
	MonsterStat = BaseMonsterStat;
	
}

void UMonsterStatComponent::UpdateEffectImage_Implementation(UTexture2D* EffectImage, int ServerEffectType, float Duration)
{
	if (UMonsterStatHUD *MonsterStatHUD = Cast<UMonsterStatHUD>(Monster->MonsterStatHUDComponent->GetWidget()))
	{
		MonsterStatHUD->AddStatusImage(EffectImage, ServerEffectType, Duration);
	}
}

void UMonsterStatComponent::SetMonsterHUDVisTrue_Implementation()
{
	Monster->MonsterStatHUDComponent->SetVisibility(true, true);
}

void UMonsterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMonsterStatComponent, BaseMonsterStat);
	DOREPLIFETIME(UMonsterStatComponent, MonsterStat);
}

void UMonsterStatComponent::die_Implementation()
{
	GetOwner()->Destroy();
}


