#include "PerkWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Info/PerkInfomation.h"
#include "EarthHero/Menu/IndexButton.h"

UPerkWidget::UPerkWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UPerkWidget::Initialize()
{
	Super::Initialize();
	
	PerkInfomations = new PerkInfomation();
	EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(PerkInfomations && EHGameInstance)
	{
		//플레이어 레벨 정보 가져오기
		Level = EHGameInstance->GetPlayerLevel();
		if(Level > 0) Point = PerkInfomations->GetPoints(Level);
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UPerkWidget::Initialize(). Level < 0"));
			Level = Point = 10; //디버그용
			//return false; //실제 있어야하는 줄
		}
		
		Level_Tb->SetText(FText::FromString(FString("Lv. ") + FString::FromInt(Level)));
		Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
		
		// 버튼 생성 및 초기화
		CreateButtons();

		//이전 저장값 불러오기
		SelectInfo = EHGameInstance->LoadGame();

		//전부 눌러주기
		int NumOfPerks = PerkInfomations->NumOfPerks;
		int64 CheckBit = 1;
		for(int i = 0; i < NumOfPerks; i++)
			if(SelectInfo & (CheckBit << i))
				Buttons[i]->IndexBtnClicked();
	}

	PerkSave_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkSaveBtnClicked);
	PerkCancel_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkCancelBtnClicked);
	
	return true;
}

void UPerkWidget::CreateButtons()
{
	int NumOfLevels = PerkInfomations->NumOfLevels;
	int NumOfPerkPerLevel = PerkInfomations->NumOfPerkPerLevel;
	
	int i, j;
	for(i = 0; i < NumOfLevels; i++)
	{
		UVerticalBox* NewVerticalBox = NewObject<UVerticalBox>(this);
		if(NewVerticalBox)
		{
			// Add the new vertical box to the horizontal box with fill settings
			UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Perk_Hb->AddChild(NewVerticalBox));
			if (HorizontalBoxSlot)
			{
				HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				HorizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}
			
			for(j = 0; j < NumOfPerkPerLevel; j++)
			{
				USizeBox* SizeBox = NewObject<USizeBox>(this);
				if (SizeBox)
				{
					SizeBox->SetWidthOverride(75);
					SizeBox->SetHeightOverride(114);
					
					UIndexButton* IndexButton = NewObject<UIndexButton>(this);
					if(IndexButton)
					{
						IndexButton->InitSetting((NumOfPerkPerLevel * i) + j, this);
						
						Buttons.Add(IndexButton);
						
						UPanelSlot* PanelSlot = SizeBox->AddChild(IndexButton);
						NewVerticalBox->AddChild(SizeBox);

						// Set SizeBoxSlot padding and fill settings
						UVerticalBoxSlot* SizeBoxSlot = Cast<UVerticalBoxSlot>(NewVerticalBox->AddChild(SizeBox));
						if(SizeBoxSlot)
						{
							SizeBoxSlot->SetPadding(FMargin(10, 10, 0, 0));
							SizeBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
							SizeBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
						}
					}
				}
			}
		}
	}

	// Disable buttons that are not applicable for the current level
	int NumOfPerks = PerkInfomations->NumOfPerks;
	for(i = Level * NumOfPerkPerLevel; i < NumOfPerks; i++)
	{
		Buttons[i]->SetIsEnabled(false);
	}
}
void UPerkWidget::UpdateSelectInfo(int Index)
{
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
	SelectInfo ^= ((int64)1 << Index);
}

void UPerkWidget::PerkButtonHovered(int Index)
{
	if(PerkInfomations && PerkInfomations->PerkDescriptions.Num() > Index)
	{
		FPerkDescription HoveredPerkInfomation = PerkInfomations->PerkDescriptions[Index];
		PerkName_Tb->SetText(HoveredPerkInfomation.Name);
		FString CostText = FString("Cost : ") + HoveredPerkInfomation.Cost.ToString();
		PerkCost_Tb->SetText(FText::FromString(CostText));
		PerkDescription_Tb->SetText(HoveredPerkInfomation.Description);
	}
}

void UPerkWidget::PerkButtonUnhovered(int Index)
{
	PerkName_Tb->SetText(FText::FromString(""));
	PerkCost_Tb->SetText(FText::FromString(""));
	PerkDescription_Tb->SetText(FText::FromString(""));
}

void UPerkWidget::PerkSaveBtnClicked()
{
	if(EHGameInstance)
	{
		EHGameInstance->PerkInfo = SelectInfo;
		EHGameInstance->SaveGame(SelectInfo);
	}

	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UPerkWidget::PerkCancelBtnClicked()
{
	int NumOfPerks = PerkInfomations->NumOfPerks;
	int64 CheckBit = 1;

	for(int i = 0; i < NumOfPerks; i++)
	{
		if(SelectInfo & (CheckBit << i))
		{
			Buttons[i]->IndexBtnClicked();
		}
	}

	SelectInfo = 0;

	Point = PerkInfomations->GetPoints(Level);
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
}
