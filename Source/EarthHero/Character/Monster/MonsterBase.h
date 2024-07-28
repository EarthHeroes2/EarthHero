// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterBase.generated.h"

class UNavigationInvokerComponent;

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

	UNavigationInvokerComponent* NavigationInvoker;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UMonsterStatComponent *MonsterStatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Type")
	int MonsterType;





	
	//임시로 만들어둠 - 박정익
	//자식에서 상속받아서 값 바꾸어주세용
	//원거리 몬스터인 경우 사거리가 시야보다 더 넓게 해주세요
	float AttackRange = 100.0f;

	//시야 거리, 목표 상실 거리, 시야각, 감각을 잃는 기간, 마지막으로 감지된 객체의 위치 탐지 성공 거리
	//기본값은 임시값
	float AISightRadius = 400.f;
	float AILoseSightRadius = 500.f;
	float AIFieldOfView = 90.f; //시야각인데 *2가 됨
	float AISightAge = 4.f;
	float AILastSeenLocation = 600.f;

	//상속받아서 구현해주기
	virtual void Attack();
	virtual void Attack(FVector SpawnNormalVector); //원거리용
};
