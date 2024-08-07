// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace BlackboardKeys
{
	TCHAR const * const TargetLocation = TEXT("TargetLocation");
	TCHAR const * const CanSeePlayer = TEXT("CanSeePlayer");
	TCHAR const * const TargetPlayer = TEXT("TargetPlayer");
	TCHAR const * const IsPlayerInAttackRange = TEXT("IsPlayerInAttackRange");
	TCHAR const * const CanMove = TEXT("CanMove");

	//중간 보스 이상급
	TCHAR const * const IsSkill1Ready = TEXT("IsSkill1Ready");
	TCHAR const * const IsSkill2Ready = TEXT("IsSkill2Ready");
	TCHAR const * const IsSkill3Ready = TEXT("IsSkill3Ready");
}