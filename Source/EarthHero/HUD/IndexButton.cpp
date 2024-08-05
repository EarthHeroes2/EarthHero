// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexButton.h"

#include "PerkWidget.h"
#include "Perk/PerkInfomation.h"

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
