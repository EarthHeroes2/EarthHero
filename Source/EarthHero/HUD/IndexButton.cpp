// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexButton.h"

#include "PerkWidget.h"

UIndexButton::UIndexButton()
{
	OnClicked.AddDynamic(this, &UIndexButton::IndexBtnClicked);
}

void UIndexButton::IndexBtnClicked()
{
	bSelected = !bSelected;
	
	if(bSelected)
	{
		if(PerkWidget->Point >= NeedPoint[Index])
		{
			UE_LOG(LogTemp, Warning, TEXT("Index = %d, Need Point = %d"), Index, NeedPoint[Index]);
			
			PerkWidget->Point -= NeedPoint[Index];
			PerkWidget->UpdateSelectInfo(Index);
		}
		else
		{
			bSelected = false;
			UE_LOG(LogTemp, Warning, TEXT("Need Point : %d  Point : %d"), NeedPoint[Index], PerkWidget->Point);
		}
	}
	else
	{
		PerkWidget->Point += NeedPoint[Index];
		PerkWidget->UpdateSelectInfo(Index);
	}
}
