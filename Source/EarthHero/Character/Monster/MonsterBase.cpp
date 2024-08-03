// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"

#include "Components/WidgetComponent.h"
#include "EarthHero/HUD/MonsterStatHUD.h"
#include "EarthHero/Stat/Monster/MonsterStatComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MonsterStatComponent = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("MonsterStatComponent"));
	MonsterStatComponent->SetIsReplicated(true);

	MonsterStatHUDComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterStatHUD"));
	MonsterStatHUDComponent->SetupAttachment(RootComponent);

	//이 값은 자식에서 변경해줘야함. 기본값을 위해 존재할 뿐임
	BossNumber = NotBoss;
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//자식에서 구현 (근거리용)
void AMonsterBase::Attack()
{
}
//자식에서 구현 (원거리용)
//SpawnNormalVector는 타겟 플레이어를 향하는 단위벡터
void AMonsterBase::Attack(FVector SpawnNormalVector)
{
}

//스킬들 (자식에서 필요하다면 구현)
void AMonsterBase::Skill1()
{
}
void AMonsterBase::Skill2()
{
}
void AMonsterBase::Skill3()
{
}
void AMonsterBase::Skill4()
{
}

void AMonsterBase::Skill1(FVector SpawnNormalVector)
{
}
void AMonsterBase::Skill2(FVector SpawnNormalVector)
{
}
void AMonsterBase::Skill3(FVector SpawnNormalVector)
{
}
void AMonsterBase::Skill4(FVector SpawnNormalVector)
{
}

void AMonsterBase::Skill1(AEHCharacter* TargetCharacter)
{
}
void AMonsterBase::Skill2(AEHCharacter* TargetCharacter)
{
}
void AMonsterBase::Skill3(AEHCharacter* TargetCharacter)
{
}
void AMonsterBase::Skill4(AEHCharacter* TargetCharacter)
{
}


