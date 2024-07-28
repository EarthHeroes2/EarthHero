// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolDown.h"

#include "InGameHUD.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCoolDown::SetImage(UTexture2D* Image)
{
	FSlateBrush Brush;
	Brush.SetResourceObject(Image);
	EffectImage->SetBrush(Brush);
	EffectImage->SetOpacity(1.f);
}

void UCoolDown::ClearImage()
{
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0)); // 투명한 색상으로 설정
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
	InGameHUD->DeleteStatusImage(EffectType);
}

