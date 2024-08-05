// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkButtonWidget.h"

#include "Components/Button.h"

void UPerkButtonWidget::AddOnClick()
{
	Perk_Btn->OnClicked.AddDynamic(this, &UPerkButtonWidget::PerkBtnClicked);
}

void UPerkButtonWidget::PerkBtnClicked()
{
	//이 버튼의 index를 이용해서..
}


