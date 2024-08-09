#include "PerkWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Menu/IndexButton.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"

UPerkWidget::UPerkWidget(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Load the DataTable at initialization using FObjectFinder
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_PerkDataTable(TEXT("/Game/Data/Perk/PerkDataTable.PerkDataTable"));
    if (DT_PerkDataTable.Succeeded())
    {
        PerkDataTable = DT_PerkDataTable.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Perk DataTable!"));
        PerkDataTable = nullptr;
    }
}

bool UPerkWidget::Initialize()
{
    Super::Initialize();

    if (!PerkDataTable)
    {
        return false;
    }

    EHGameInstance = Cast<UEHGameInstance>(GetGameInstance());
    if (!EHGameInstance)
    {
        return false;
    }

    // Load player level and points
    Level = EHGameInstance->GetPlayerLevel();
    if (Level > 0)
    {
        Point = Level + 2;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UPerkWidget::Initialize(). Level < 0"));
        Point = Level = 10;
        //return false;
    }

    Level_Tb->SetText(FText::FromString(FString("Lv. ") + FString::FromInt(Level)));
    Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));

    // Create and initialize buttons
    CreateButtons();

    // Load previous saved data
    SelectInfo = EHGameInstance->LoadGame();

    // Apply previous selections
    int NumOfPerks = PerkDataTable->GetRowNames().Num();
    int64 CheckBit = 1;
    for (int i = 0; i < NumOfPerks; i++)
        if (SelectInfo & (CheckBit << i))
            Buttons[i]->IndexBtnClicked();

    PerkSave_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkSaveBtnClicked);
    PerkCancel_Btn->OnClicked.AddDynamic(this, &UPerkWidget::PerkCancelBtnClicked);

    return true;
}

void UPerkWidget::CreateButtons()
{
    if (!PerkDataTable)
    {
        return;
    }

    int NumOfLevels = 10;
    int NumOfPerkPerLevel = 5;
    TArray<FName> RowNames = PerkDataTable->GetRowNames();

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
                        int Index = (NumOfPerkPerLevel * i) + j;
                        IndexButton->InitSetting(Index, this, PerkDataTable);

                        Buttons.Add(IndexButton);

                        if (RowNames.IsValidIndex(Index))
                        {
                            FPerkStructure* PerkData = PerkDataTable->FindRow<FPerkStructure>(RowNames[Index], TEXT("Perk Widget"));
                            if (PerkData && PerkData->Image)
                            {
                                FSlateBrush Brush;
                                Brush.SetResourceObject(PerkData->Image);
                                Brush.ImageSize = FVector2D(60.0f, 60.0f);

                                IndexButton->WidgetStyle.Normal.SetResourceObject(PerkData->Image);
                                IndexButton->WidgetStyle.Hovered.SetResourceObject(PerkData->Image);
                                IndexButton->WidgetStyle.Pressed.SetResourceObject(PerkData->Image);
                            }
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

    // Disable buttons if the level does not match
    int NumOfPerks = PerkDataTable->GetRowNames().Num();
    for (int i = Level * NumOfPerkPerLevel; i < NumOfPerks; i++)
    {
        Buttons[i]->SetIsEnabled(false);
    }
}

void UPerkWidget::AddEquippedPerkImage(UTexture2D* PerkTexture)
{
    if (!PerkTexture)
    {
    	UE_LOG(LogTemp, Warning, TEXT("PerkTexture is null in RemoveEquippedPerkImage!"));
        return;
    }

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

void UPerkWidget::RemoveEquippedPerkImage(UTexture2D* PerkTexture)
{
    if (!PerkTexture)
    {
        UE_LOG(LogTemp, Warning, TEXT("PerkTexture is null in RemoveEquippedPerkImage!"));
        return;
    }

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

void UPerkWidget::UpdateSelectInfo(int Index)
{
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
	SelectInfo ^= ((int64)1 << Index);
}

void UPerkWidget::PerkButtonHovered(int Index)
{
	if (PerkDataTable)
	{
		TArray<FName> RowNames = PerkDataTable->GetRowNames();
		if (RowNames.IsValidIndex(Index))
		{
			FPerkStructure* HoveredPerk = PerkDataTable->FindRow<FPerkStructure>(RowNames[Index], TEXT("Perk Hover"));
			if (HoveredPerk)
			{
				PerkName_Tb->SetText(HoveredPerk->PerkName);
				FString CostText = FString("Cost : ") + FString::FromInt(HoveredPerk->Cost);
				PerkCost_Tb->SetText(FText::FromString(CostText));
				PerkDescription_Tb->SetText(HoveredPerk->Description);

				if (HoveredPerk->Image)
				{
					PerkImage_Img->SetBrushFromTexture(HoveredPerk->Image);
					PerkImage_Img->SetVisibility(ESlateVisibility::Visible);
				}
			}
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
	int NumOfPerks = PerkDataTable->GetRowNames().Num();
	int64 CheckBit = 1;

	for(int i = 0; i < NumOfPerks; i++)
	{
		if(SelectInfo & (CheckBit << i))
		{
			Buttons[i]->IndexBtnClicked();
		}
	}

	SelectInfo = 0;

	Point = Level + 2;
	Point_Tb->SetText(FText::FromString(FString("Point. ") + FString::FromInt(Point)));
}
