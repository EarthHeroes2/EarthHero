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
#include "EarthHero/Info/PerkInformation.h"
#include "EarthHero/Menu/IndexButton.h"

UPerkWidget::UPerkWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UPerkWidget::Initialize()
{
	Super::Initialize();
	
	PerkInformations = new PerkInformation();
	EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
	if(PerkInformations && EHGameInstance)
	{
		//플레이어 레벨 정보 가져오기
		Level = EHGameInstance->GetPlayerLevel();
		if(Level > 0) Point = PerkInformations->GetPoints(Level);
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
		int NumOfPerks = PerkInformations->NumOfPerks;
		int64 CheckBit = 1;
		for(int i = 0; i < NumOfPerks; i++)
			if(SelectInfo & (CheckBit << i))
				Buttons[i]->IndexBtnClicked();

		// 이미지 바꿔주기
		PerkInformations->SetImages();
	}

	PerkSave_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkSaveBtnClicked);
	PerkCancel_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkCancelBtnClicked);
	
	return true;
}

void UPerkWidget::CreateButtons()
{
	int NumOfLevels = PerkInformations->NumOfLevels;
	int NumOfPerkPerLevel = PerkInformations->NumOfPerkPerLevel;
	
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
	int NumOfPerks = PerkInformations->NumOfPerks;
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
	if(PerkInformations && PerkInformations->PerkDescriptions.Num() > Index)
	{
		FPerkDescription HoveredPerkInformation = PerkInformations->PerkDescriptions[Index];
		PerkName_Tb->SetText(HoveredPerkInformation.Name);
		FString CostText = FString("Cost : ") + HoveredPerkInformation.Cost.ToString();
		PerkCost_Tb->SetText(FText::FromString(CostText));
		PerkDescription_Tb->SetText(HoveredPerkInformation.Description);
		PerkImage_Img = HoveredPerkInformation.Image;
	}
}

void UPerkWidget::PerkButtonUnhovered(int Index)
{
	PerkName_Tb->SetText(FText::FromString(""));
	PerkCost_Tb->SetText(FText::FromString(""));
	PerkDescription_Tb->SetText(FText::FromString(""));
	PerkImage_Img->SetBrush(FSlateBrush());
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
	int NumOfPerks = PerkInformations->NumOfPerks;
	int64 CheckBit = 1;

	for(int i = 0; i < NumOfPerks; i++)
	{
		if(SelectInfo & (CheckBit << i))
		{
			Buttons[i]->IndexBtnClicked();
		}
	}

	SelectInfo = 0;

	Point = PerkInformations->GetPoints(Level);
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
}
