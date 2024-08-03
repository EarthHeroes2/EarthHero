// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Character/EHCharacter.h"
#include "EarthHero/Enum/Enums.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

UCLASS()
class EARTHHERO_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UMonsterStatComponent *MonsterStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UWidgetComponent *MonsterStatHUDComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Type")
	int MonsterType;


	
	//자식에서 상속받아서 값 바꾸어주세요 - 박정익
	//원거리 몬스터인 경우 사거리가 시야보다 더 넓게 해주세요
	//시야는 ai 컨트롤러에서 상속받아서 처리합니다
	float AttackRange = 100.0f;

	//기본공격
	//상속받아서 구현해주기
	virtual void Attack(); //근거리용
	virtual void Attack(FVector SpawnNormalVector); //원거리용

	//보스의 번호를 의미
	//상속받아서 값을 지정해줘야함
	EBossName BossNumber;

	//스킬
	//상속받아서 구현해주기
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();
	virtual void Skill1(FVector SpawnNormalVector); //향할 단위벡터
	virtual void Skill2(FVector SpawnNormalVector);
	virtual void Skill3(FVector SpawnNormalVector);
	virtual void Skill4(FVector SpawnNormalVector);
	virtual void Skill1(AEHCharacter* TargetCharacter); //타겟팅 대상
	virtual void Skill2(AEHCharacter* TargetCharacter);
	virtual void Skill3(AEHCharacter* TargetCharacter);
	virtual void Skill4(AEHCharacter* TargetCharacter);
};
