// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorStatComponent.h"

//워리어 생성자
UWarriorStatComponent::UWarriorStatComponent()
{
}

void UWarriorStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWarriorStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

//워리어 데미지 받는 함수(오버라이드)


//워리어 스텟 초기화 함수(오버라이드)
void UWarriorStatComponent::InitializeStatData_Implementation(FName HeroName)
{
	Super::InitializeStatData_Implementation(HeroName);
}


////////////////
/*워리어 용 함수*/
///////////////


//워리어 일반데미지
void UWarriorStatComponent::WarriorDamage_Implementation(AActor* DamagedActor,  TSubclassOf<UDamageType> DamageTypeClass)
{
}

//워리어 휠윈드
void UWarriorStatComponent::WarriorWheelWindDamage_Implementation(AActor* DamagedActor,  TSubclassOf<UDamageType> DamageTypeClass)
{
}

//워리어 대쉬
void UWarriorStatComponent::WarriorDashDamage_Implementation(AActor* DamagedActor,  TSubclassOf<UDamageType> DamageTypeClass)
{
}
