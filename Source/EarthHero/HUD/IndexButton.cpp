// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexButton.h"

#include "PerkWidget.h"
#include "EarthHero/EHGameInstance.h"

UIndexButton::UIndexButton()
{
	EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		NeedPoint = EHGameInstance->NeedPoint[Index];
		OnClicked.AddDynamic(this, &UIndexButton::IndexBtnClicked);
	}
}

void UIndexButton::IndexBtnClicked()
{
	if(EHGameInstance == nullptr) return;
	
	bSelected = !bSelected;
	
	if(bSelected)
	{
		if(PerkWidget->Point >= NeedPoint)
		{
			SetRenderOpacity(0.5);
			
			PerkWidget->Point -= NeedPoint;
			PerkWidget->UpdateSelectInfo(Index);
		}
		else
		{
			bSelected = false;
		}
	}
	else
	{
		SetRenderOpacity(1);
		
		PerkWidget->Point += NeedPoint;
		PerkWidget->UpdateSelectInfo(Index);
	}
}
