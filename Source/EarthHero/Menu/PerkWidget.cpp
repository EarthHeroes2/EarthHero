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
	}

	PerkSave_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkSaveBtnClicked);
	PerkCancel_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkCancelBtnClicked);
	
	return true;
}

void UPerkWidget::CreateButtons()
{
	int NumOfLevels = PerkInformations->NumOfLevels;
	int NumOfPerkPerLevel = PerkInformations->NumOfPerkPerLevel;
	
	for (int i = 0; i < NumOfLevels; i++)
	{
		UVerticalBox* NewVerticalBox = NewObject<UVerticalBox>(this);
		if (NewVerticalBox)
		{
			UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(Perk_Hb->AddChild(NewVerticalBox));
			if (HorizontalBoxSlot)
			{
				HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				HorizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			}
			
			for (int j = 0; j < NumOfPerkPerLevel; j++)
			{
				USizeBox* SizeBox = NewObject<USizeBox>(this);
				if (SizeBox)
				{
					SizeBox->SetWidthOverride(60);
					SizeBox->SetHeightOverride(80);
					
					UIndexButton* IndexButton = NewObject<UIndexButton>(this);
					if (IndexButton)
					{
						IndexButton->InitSetting((NumOfPerkPerLevel * i) + j, this);
						
						Buttons.Add(IndexButton);

						UTexture2D* ButtonImage = PerkInformations->PerkDescriptions[(NumOfPerkPerLevel * i) + j].Image;
						if (ButtonImage)
						{
							FSlateBrush Brush;
							Brush.SetResourceObject(ButtonImage);
							Brush.ImageSize = FVector2D(60.0f, 60.0f);

							IndexButton->WidgetStyle.Normal.SetResourceObject(ButtonImage);
							IndexButton->WidgetStyle.Hovered.SetResourceObject(ButtonImage);
							IndexButton->WidgetStyle.Pressed.SetResourceObject(ButtonImage);
						}

						UPanelSlot* PanelSlot = SizeBox->AddChild(IndexButton);
						NewVerticalBox->AddChild(SizeBox);

						UVerticalBoxSlot* SizeBoxSlot = Cast<UVerticalBoxSlot>(NewVerticalBox->AddChild(SizeBox));
						if (SizeBoxSlot)
						{
							SizeBoxSlot->SetPadding(FMargin(20, 30, 0, 0));
							SizeBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
							SizeBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
						}
					}
				}
			}
		}
	}

	// 현재 레벨에 맞지 않으면 꺼둠
	int NumOfPerks = PerkInformations->NumOfPerks;
	for (int i = Level * NumOfPerkPerLevel; i < NumOfPerks; i++)
	{
		Buttons[i]->SetIsEnabled(false);
	}
}

void UPerkWidget::RemoveEquippedPerkImage(UTexture2D* PerkTexture)
{
	if (EquippedPerks_Hb)
	{
		for (int32 i = 0; i < EquippedPerks_Hb->GetChildrenCount(); i++)
		{
			UWidget* ChildWidget = EquippedPerks_Hb->GetChildAt(i);
			UImage* ImageWidget = Cast<UImage>(ChildWidget);

			if (ImageWidget && ImageWidget->Brush.GetResourceObject() == PerkTexture)
			{
				EquippedPerks_Hb->RemoveChildAt(i);
				break;
			}
		}
	}
}

void UPerkWidget::AddEquippedPerkImage(UTexture2D* PerkTexture)
{
	if (PerkTexture)
	{
		UImage* NewImage = NewObject<UImage>(this);

		FSlateBrush Brush;
		Brush.SetResourceObject(PerkTexture);
		Brush.ImageSize = FVector2D(120.0f, 120.0f);
		NewImage->SetBrush(Brush);

		if (EquippedPerks_Hb)
		{
			UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(EquippedPerks_Hb->AddChild(NewImage));
			if (HorizontalBoxSlot)
			{
				HorizontalBoxSlot->SetPadding(FMargin(0.0f, 0.0f, 10.0f, 0.0f));
				HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
			}
		}
	}
}

void UPerkWidget::UpdateSelectInfo(int Index)
{
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
	SelectInfo ^= ((int64)1 << Index);
}

void UPerkWidget::PerkButtonHovered(int Index)
{
	if (PerkInformations && PerkInformations->PerkDescriptions.Num() > Index)
	{
		PerkImage_Img->SetVisibility(ESlateVisibility::Visible);
		FPerkDescription HoveredPerkInformation = PerkInformations->PerkDescriptions[Index];
		PerkName_Tb->SetText(HoveredPerkInformation.Name);
		FString CostText = FString("Cost : ") + HoveredPerkInformation.Cost.ToString();
		PerkCost_Tb->SetText(FText::FromString(CostText));
		PerkDescription_Tb->SetText(HoveredPerkInformation.Description);

		if (HoveredPerkInformation.Image)
		{
			PerkImage_Img->SetBrushFromTexture(HoveredPerkInformation.Image);
		}
	}
}

void UPerkWidget::PerkButtonUnhovered(int Index)
{
	PerkName_Tb->SetText(FText::FromString(""));
	PerkCost_Tb->SetText(FText::FromString(""));
	PerkDescription_Tb->SetText(FText::FromString(""));
	PerkImage_Img->SetVisibility(ESlateVisibility::Hidden);
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
