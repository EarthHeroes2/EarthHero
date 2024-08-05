// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexButton.h"

#include "PerkWidget.h"

UIndexButton::UIndexButton()
{
	OnClicked.AddDynamic(this, &UIndexButton::IndexBtnClicked);
}

void UIndexButton::IndexBtnClicked()
{
	PerkWidget->UpdateSelectInfo(Index);
}
