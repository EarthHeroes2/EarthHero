// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkWidget.h"

#include "IndexButton.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBox.h"
#include "EarthHero/EHGameInstance.h"


UPerkWidget::UPerkWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{

	UEHGameInstance* EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		int Level = EHGameInstance->GetPlayerLevel();
		
	}
}

void UPerkWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 생성 및 초기화
	CreateButtons();

	PerkSave_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkSaveBtnClicked);
	PerkCancel_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkCancelBtnClicked);
}


void UPerkWidget::CreateButtons()
{
	int i, j;
	for(i = 0; i < NumOfLevels; i++)
	{
		UVerticalBox* NewVerticalBox = NewObject<UVerticalBox>(this);
		Perk_Hb->AddChild(NewVerticalBox);
		
		if(NewVerticalBox)
		{
			for(j = 0; j < NumOfPerkPerLevel; j++)
			{
				USizeBox* SizeBox = NewObject<USizeBox>(this);
				if (SizeBox)
				{
					SizeBox->SetWidthOverride(50);
					SizeBox->SetHeightOverride(50);
					
					UIndexButton* IndexButton = NewObject<UIndexButton>(this);
					if(IndexButton)
					{
						IndexButton->Index = (NumOfLevels * i) + j;
						IndexButton->PerkWidget = this;
						
						Buttons.Add(IndexButton);
						
						UPanelSlot* PanelSlot = SizeBox->AddChild(IndexButton);
						NewVerticalBox->AddChild(SizeBox);
						
						USizeBoxSlot* SizeBoxSlot = Cast<USizeBoxSlot>(PanelSlot);
						if(SizeBoxSlot) SizeBoxSlot->SetPadding(FMargin(10,10,0,0));
					}
				}
			}
		}
	}
}

void UPerkWidget::UpdateSelectInfo(int Index)
{
	SelectInfo ^= ((int64)1 << Index);
}


void UPerkWidget::PerkSaveBtnClicked()
{
	
}

void UPerkWidget::PerkCancelBtnClicked()
{
	
}

