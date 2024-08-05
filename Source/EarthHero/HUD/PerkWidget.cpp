// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkWidget.h"

#include "IndexButton.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "EarthHero/EHGameInstance.h"


UPerkWidget::UPerkWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UPerkWidget::Initialize()
{
	Super::Initialize();

	// 버튼 생성 및 초기화
	CreateButtons();
	
	EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(EHGameInstance)
	{
		//플레이어 레벨 정보 가져오기
		Level = EHGameInstance->GetPlayerLevel();
		if(Level > 0) Point = Level + 2;
		else Point = -1;
		
		Level_Tb->SetText(FText::FromString(FString("Lv. ") + FString::FromInt(Level)));
		Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));

		//이전 저장값 불러오기
		SelectInfo = EHGameInstance->LoadGame();

		//전부 눌러주기
		int64 CheckBit = 1;
		for(int i = 0; i < 50; i++)
			if(SelectInfo & (CheckBit << i))
				Buttons[i]->IndexBtnClicked();
	}

	PerkSave_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkSaveBtnClicked);
	PerkCancel_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkCancelBtnClicked);
	
	return true;
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
						IndexButton->InitSetting((NumOfPerkPerLevel * i) + j, this);
						
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
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
	SelectInfo ^= ((int64)1 << Index);
}

void UPerkWidget::PerkSaveBtnClicked()
{
	if(EHGameInstance)
	{
		EHGameInstance->PerkInfo = SelectInfo;
		EHGameInstance->SaveGame(SelectInfo);
	}
}

void UPerkWidget::PerkCancelBtnClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

