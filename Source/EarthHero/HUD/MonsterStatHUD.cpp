// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatHUD.h"

#include "Components/ProgressBar.h"
#include "Structure/Status.h"

bool UMonsterStatHUD::Initialize()
{
	Super::Initialize();
	
	StatusArray.Add(FStatus(0, Status));
	StatusArray.Add(FStatus(0, Status_1));
	StatusArray.Add(FStatus(0, Status_2));
	StatusArray.Add(FStatus(0, Status_3));
	StatusArray.Add(FStatus(0, Status_4));
	StatusArray.Add(FStatus(0, Status_5));
	StatusArray.Add(FStatus(0, Status_6));
	StatusArray.Add(FStatus(0, Status_7));
	return true;
}

void UMonsterStatHUD::UpdateHealth(float HeathPercent)
{
	HealthBar->SetPercent(HeathPercent);
}

void UMonsterStatHUD::AddStatusImage(UTexture2D* EffectImage, int EffectType, float CoolDown)
{
	EffectCount += 1;
	//UE_LOG(LogClass, Warning, TEXT("Monster EffectType = %d, EffectCount = %d"), EffectType, EffectCount);
	StatusArray[EffectCount - 1] = FStatus(EffectType, StatusArray[EffectCount - 1].CoolDownWidget);
	StatusArray[EffectCount - 1].CoolDownWidget->SetImage(EffectImage);
	StatusArray[EffectCount - 1].CoolDownWidget->StartCoolDown(CoolDown, EffectType);
	StatusArray[EffectCount - 1].CoolDownWidget->MonsterStatHUD = this;
}

void UMonsterStatHUD::DeleteStatusImage(int EffectType)
{
	EffectCount -= 1;
	FStatus *FindStatus = Algo::FindByPredicate(StatusArray, [EffectType](const FStatus& Status)
	{
		return Status.EffectType == EffectType;
	});
	FindStatus->CoolDownWidget->ClearImage();
}