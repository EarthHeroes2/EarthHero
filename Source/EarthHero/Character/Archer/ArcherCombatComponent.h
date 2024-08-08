// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArcherCombatComponent.generated.h"


class AEHArcher;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EARTHHERO_API UArcherCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UArcherCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	AEHArcher* Archer;

public:
	void SetArcher(AEHArcher* NewArcher) { Archer = NewArcher; }
};
