// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolDown.h"

#include "InGameHUD.h"
#include "InGamePlayerInfo.h"
#include "MonsterStatHUD.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCoolDown::SetImage(UTexture2D* Image)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	Brush.ImageSize = FVector2D(100.f, 100.f);
	EffectImage->SetBrush(Brush);
	EffectImage->SetOpacity(1.f);
}

void UCoolDown::ClearImage()
{
	FSlateBrush Brush;
	Brush.ImageSize = FVector2D(100.f, 100.f);
	Brush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	EffectImage->SetBrush(Brush);
}

void UCoolDown::StartCoolDown(float CoolDown, int InEffectType)
{
	GetWorld()->GetTimerManager().ClearTimer(ProgressHandle);
	GetWorld()->GetTimerManager().ClearTimer(EffectEndHandle);
	EffectType = InEffectType;
	SubtractPercent = 0.1f / CoolDown;
	RemainingTime = CoolDown;
	Percent = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(ProgressHandle, this, &UCoolDown::Loop, 0.1f, true);
	GetWorld()->GetTimerManager().SetTimer(EffectEndHandle, this, &UCoolDown::End, CoolDown, false);
}

void UCoolDown::UpdateCoolDown(float CoolDown)
{
	GetWorld()->GetTimerManager().ClearTimer(ProgressHandle);
	GetWorld()->GetTimerManager().ClearTimer(EffectEndHandle);

	//ClearImage();
	
	RemainingTime = CoolDown;
	SubtractPercent = 0.1f / CoolDown;
	Percent = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(ProgressHandle, this, &UCoolDown::Loop, 0.1f, true);
	GetWorld()->GetTimerManager().SetTimer(EffectEndHandle, this, &UCoolDown::End, CoolDown, false);
}

void UCoolDown::Loop()
{
	RemainingTime -= 0.1f;
	Percent -= SubtractPercent;
	ProgressBar_CoolDown->SetPercent(Percent);
	Text_RemainingTime->SetText(FText::AsNumber(RemainingTime));
}

void UCoolDown::End()
{
	GetWorld()->GetTimerManager().ClearTimer(ProgressHandle);
	GetWorld()->GetTimerManager().ClearTimer(EffectEndHandle);
	ProgressBar_CoolDown->SetPercent(0.f);
	Text_RemainingTime->SetText(FText::GetEmpty());
	if (InGameHUD)
	{
		InGameHUD->DeleteStatusImage(EffectType);
	}
	else if (InGamePlayerInfo)
	{
		InGamePlayerInfo->DeletePlayerEffects(EffectType);
	}
	else if (MonsterStatHUD)
	{
		MonsterStatHUD->DeleteStatusImage(EffectType);
	}
}

