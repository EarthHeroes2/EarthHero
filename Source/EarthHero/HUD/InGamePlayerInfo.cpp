// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerInfo.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EarthHero/Stat/Effect/EffectBase.h"

void UInGamePlayerInfo::SetName(const FString& Name) const
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(Name));
	}
}

void UInGamePlayerInfo::SetProgress(const float Progress) const
{
	UE_LOG(LogClass, Warning, TEXT("InGamePlayInfo progress = %f"), Progress);
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Progress);
	}
}

void UInGamePlayerInfo::SetImage(UTexture2D *Image) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	PlayerClass->SetBrush(Brush);
}

void UInGamePlayerInfo::UpdatePlayerEffects(FEffectStatus PlayerStatus)
{
	int Index = 0;
	if ((Index = CheckEffectTypeAlreadyExists(PlayerStatus.EffectType)) == -1)
	{
		EffectCount += 1;
		//UE_LOG(LogClass, Warning, TEXT("InGamePlayerInfo: EffectType = %d, EffectDuration = %f, EffectCount = %d"), PlayerStatus.EffectType, PlayerStatus.EffectDuration, EffectCount);
		PLayerStatusArray[EffectCount - 1] = FStatus(PlayerStatus.EffectType, PLayerStatusArray[EffectCount - 1].CoolDownWidget);
		PLayerStatusArray[EffectCount - 1].CoolDownWidget->SetImage(PlayerStatus.EffectImage);
		PLayerStatusArray[EffectCount - 1].CoolDownWidget->StartCoolDown(PlayerStatus.EffectDuration, PlayerStatus.EffectType);
		PLayerStatusArray[EffectCount - 1].CoolDownWidget->InGamePlayerInfo = this;
	}
	else
	{
		PLayerStatusArray[Index].CoolDownWidget->UpdateCoolDown(PlayerStatus.EffectDuration);
	}
}

int UInGamePlayerInfo::CheckEffectTypeAlreadyExists(int EffectType)
{
	for (int index = 0; index < EffectCount; index++)
	{
		if (PLayerStatusArray[index].EffectType == EffectType)
		{
			return index;
		}
	}
	return -1;
}

void UInGamePlayerInfo::DeletePlayerEffects(int EffectType)
{
	EffectCount -= 1;
	FStatus *FindStatus = Algo::FindByPredicate(PLayerStatusArray, [EffectType](const FStatus& Status)
	{
		return Status.EffectType == EffectType;
	});
	FindStatus->CoolDownWidget->ClearImage();
}

bool UInGamePlayerInfo::Initialize()
{
	Super::Initialize();

	PLayerStatusArray.Add(FStatus(0, Player_Status));
	PLayerStatusArray.Add(FStatus(0, Player_Status_1));
	PLayerStatusArray.Add(FStatus(0, Player_Status_2));
	PLayerStatusArray.Add(FStatus(0, Player_Status_3));
	PLayerStatusArray.Add(FStatus(0, Player_Status_4));
	PLayerStatusArray.Add(FStatus(0, Player_Status_5));
	PLayerStatusArray.Add(FStatus(0, Player_Status_6));
	PLayerStatusArray.Add(FStatus(0, Player_Status_7));
	PLayerStatusArray.Add(FStatus(0, Player_Status_8));
	PLayerStatusArray.Add(FStatus(0, Player_Status_9));
	PLayerStatusArray.Add(FStatus(0, Player_Status_10));

	return true;
}
