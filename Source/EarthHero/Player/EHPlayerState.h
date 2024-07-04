// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EarthHero/Enum/Enums.h"
#include "GameFramework/PlayerState.h"
#include "EHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API AEHPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEHPlayerState();
	
	virtual void CopyProperties(APlayerState* PlayerState) override;

	//승언: 임시 컴포넌트 추가, 각 히어로 별 컴포넌트로 대체 예정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UStatComponent* StatComponent;
	
	EClassType PlayerClass;
};