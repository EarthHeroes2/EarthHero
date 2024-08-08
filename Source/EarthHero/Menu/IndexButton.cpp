#include "IndexButton.h"

#include "PerkWidget.h"
#include "Components/TextBlock.h"
#include "EarthHero/Info/PerkInformation.h"

void UIndexButton::InitSetting(int ReceivedIndex, UPerkWidget* ParentWidget)
{
	Index = ReceivedIndex;
	PerkWidget = ParentWidget;
	
	PerkInfo = new PerkInformation();
	if(PerkInfo)
	{
		NeedPoint = PerkInfo->NeedPoint[Index];
		OnClicked.AddDynamic(this, &UIndexButton::IndexBtnClicked);
	}
	
	OnHovered.AddDynamic(this, &UIndexButton::ButtonHovered);
	OnUnhovered.AddDynamic(this, &UIndexButton::ButtonUnhovered);

	if((PerkInfo->NumOfLevels - 1) == (Index / PerkInfo->NumOfPerkPerLevel)) bHCode = true;
}

void UIndexButton::IndexBtnClicked()
{
    if (NeedPoint < 0 || PerkInfo == nullptr || PerkWidget == nullptr) return;

    bool bCanSelect = bSelected || PerkWidget->SelectedPerksCount < PerkWidget->MaxSelectedPerks;
    
    if (!bCanSelect && !bSelected)
    {
        return;
    }

    bSelected = !bSelected;

    if (bSelected)
    {
        if (PerkWidget->Point >= NeedPoint)
        {
            SetRenderOpacity(0.5);
            PerkWidget->Point -= NeedPoint;
            PerkWidget->UpdateSelectInfo(Index);

            // 장착한 퍽 Hb에 추가
            PerkWidget->AddEquippedPerkImage(PerkInfo->PerkDescriptions[Index].Image);

            // 선택된 퍽 개수 증가
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

    	// 선택된 퍽 개수 감소
        PerkWidget->SelectedPerksCount--;

        PerkWidget->RemoveEquippedPerkImage(PerkInfo->PerkDescriptions[Index].Image);
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