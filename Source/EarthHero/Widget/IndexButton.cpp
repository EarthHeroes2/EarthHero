// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexButton.h"

#include "PerkWidget.h"
#include "Components/TextBlock.h"
#include "EarthHero/Info/Perk/PerkInfomation.h"

void UIndexButton::InitSetting(int ReceivedIndex, UPerkWidget* ParentWidget)
{
	Index = ReceivedIndex;
	PerkWidget = ParentWidget;
	
	PerkInfomation* PerkInfo = new PerkInfomation();
	if(PerkInfo)
	{
		NeedPoint = PerkInfo->NeedPoint[Index];
		OnClicked.AddDynamic(this, &UIndexButton::IndexBtnClicked);
	}
	
	OnHovered.AddDynamic(this, &UIndexButton::ButtonHovered);
	OnUnhovered.AddDynamic(this, &UIndexButton::ButtonUnhovered);
}


void UIndexButton::IndexBtnClicked()
{
	if(NeedPoint < 0) return;
	
	bSelected = !bSelected;
	
	if(bSelected)
	{
		if(PerkWidget->Point >= NeedPoint)
		{
			SetRenderOpacity(0.5);
			
			PerkWidget->Point -= NeedPoint;
			PerkWidget->UpdateSelectInfo(Index);
		}
		else bSelected = false;
	}
	else
	{
		SetRenderOpacity(1);
		
		PerkWidget->Point += NeedPoint;
		PerkWidget->UpdateSelectInfo(Index);
	}
}


void UIndexButton::ButtonHovered()
{
	if(PerkWidget) PerkWidget->PerkButtonHovered(Index);
}

void UIndexButton::ButtonUnhovered()
{
	if(PerkWidget) PerkWidget->PerkButtonUnhovered(Index);
}