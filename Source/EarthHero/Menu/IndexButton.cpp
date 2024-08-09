#include "IndexButton.h"
#include "PerkWidget.h"
#include "EarthHero/Stat/Structure/PerkStructure.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"

void UIndexButton::InitSetting(int ReceivedIndex, UPerkWidget* ParentWidget, UDataTable* DataTable)
{
	Index = ReceivedIndex;
	PerkWidget = ParentWidget;
	PerkDataTable = DataTable;
	
	if (PerkDataTable)
	{
		TArray<FName> RowNames = PerkDataTable->GetRowNames();
		if (RowNames.IsValidIndex(Index))
		{
			FPerkStructure* PerkData = PerkDataTable->FindRow<FPerkStructure>(RowNames[Index], TEXT("IndexButton Init"));
			if (PerkData)
			{
				NeedPoint = PerkData->Cost;
				OnClicked.AddDynamic(this, &UIndexButton::IndexBtnClicked);

				// H 레벨
				bHCode = ((ParentWidget->Level == 9) && (Index >= 45 && Index < 50));
			}
		}
	}
	
	OnHovered.AddDynamic(this, &UIndexButton::ButtonHovered);
	OnUnhovered.AddDynamic(this, &UIndexButton::ButtonUnhovered);
}

void UIndexButton::IndexBtnClicked()
{
    if (NeedPoint < 0 || PerkDataTable == nullptr || PerkWidget == nullptr) return;

    bool bCanSelect = bSelected || PerkWidget->SelectedPerksCount < PerkWidget->MaxSelectedPerks;
    
    if (!bCanSelect && !bSelected)
    {
        return;
    }

    bSelected = !bSelected;

    TArray<FName> RowNames = PerkDataTable->GetRowNames();
    if (!RowNames.IsValidIndex(Index)) return;

    FPerkStructure* PerkData = PerkDataTable->FindRow<FPerkStructure>(RowNames[Index], TEXT("IndexButton Click"));

    if (bSelected)
    {
        if (PerkWidget->Point >= NeedPoint)
        {
            SetRenderOpacity(0.5);
            PerkWidget->Point -= NeedPoint;
            PerkWidget->UpdateSelectInfo(Index);

            // Add equipped perk image
            if (PerkData && PerkData->Image)
            {
                PerkWidget->AddEquippedPerkImage(PerkData->Image);
            }

            // Increment selected perks count
            PerkWidget->SelectedPerksCount++;
        }
        else
        {
            bSelected = false;
        }
    }
    else
    {
        SetRenderOpacity(1);
        PerkWidget->Point += NeedPoint;
        PerkWidget->UpdateSelectInfo(Index);

        // Decrement selected perks count
        PerkWidget->SelectedPerksCount--;

        if (PerkData && PerkData->Image)
        {
            PerkWidget->RemoveEquippedPerkImage(PerkData->Image);
        }
    }
}

void UIndexButton::ButtonHovered()
{
	if(PerkWidget) PerkWidget->PerkButtonHovered(Index);
}

void UIndexButton::ButtonUnhovered()
{
	if(PerkWidget) PerkWidget->PerkButtonUnhovered(Index);
}
