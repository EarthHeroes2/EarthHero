// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StatStructure.generated.h"

USTRUCT(BlueprintType)
struct FStatStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Stats")
	float RequiresExp[10] = {0, 20, 50, 120, 300, 750, 1800, 3600, 6000, 10000};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth = 100.f;				//최대 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthRegeneration = 0.f;			//체력 재생

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MovementSpeed = 1.f;				//이동속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float NormalDamage = 1.f;				//일반 공격 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackSpeed = 1.f;				//공격 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SkillDamage = 1.f;				//스킬 데미지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxExp = 100.f;					//최대 경험치

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Exp = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float SkillCoolTime = 0.f;				//공격 스킬 쿨타임 감소

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DashCoolTime = 0.f;				//이동 스킬 쿨타임 감소

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float DefensePower = 0.f;				//보호막

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float IncreasedExpGain = 0.f;			//경험치 획득량 증가

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float JumpPower = 0.f;					//점프력
	
};
