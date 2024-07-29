// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"

#include "EarthHero/EHGameInstance.h"
#include "EarthHero/AI/AIController/AIControllerBase.h"
#include "EarthHero/Character/Monster/MonsterBase.h"
#include "EarthHero/GameMode/PlayingGameMode.h"
#include "EarthHero/Stat/StatCalculationLibrary.h"
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
	
	InitializeStatData();
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
	
	FString Message = FString::Printf(TEXT("Health : %f"), MonsterStat.Health);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Message);

	AActor* Actor = GetOwner();
	if(Actor)
	{
		AMonsterBase* Monster = Cast<AMonsterBase>(Actor);
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
	}


	
	if (MonsterStat.Health <= 0.f)
	{
		IsDead = true;
		FString DeadMessage = FString::Printf(TEXT("Monster Dead"));
		APlayingGameMode *GameMode = Cast<APlayingGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			// 몬스터 죽인 수 증가
			//GameMode->AddPlayerDead();
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, DeadMessage);
	}

	return resultDamage;
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
	if (FStatStructure *TempStat = ABGameInstance->GetStatStructure("NormalMonster"))
	{
		BaseMonsterStat = *TempStat;
		BaseMonsterStat.Health = BaseMonsterStat.MaxHealth;
		MonsterStat = BaseMonsterStat;
	}
	else
	{
		// 데이터 테이블에 없는 행일 때
		//GEngine->AddOnScreenDebugMessage(-1, 1233223.f, FColor::Green, TEXT("Row %s data doesn't exist"), *HeroName.ToString());
		UE_LOG(LogClass, Warning, TEXT("Row 'Hero' data doesn't exist."));
	}
}


void UMonsterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMonsterStatComponent, BaseMonsterStat);
	DOREPLIFETIME(UMonsterStatComponent, MonsterStat);
}

