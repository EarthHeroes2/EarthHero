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
	if(NeedPoint < 0) return;
	if(PerkInfo == nullptr) return;
	if(PerkWidget == nullptr) return;
	
	bSelected = !bSelected;
	
	//HCode는 단 하나만 찍을 수 있음
	if(bHCode)
	{
		if(!(PerkWidget->bHCodeSelected)) //h코드를 선택한 게 없다면
		{
			if(bSelected)
			{
				if(PerkWidget->Point >= NeedPoint)
				{
					SetRenderOpacity(0.5);

					PerkWidget->bHCodeSelected = true;
					PerkWidget->Point -= NeedPoint;
					PerkWidget->UpdateSelectInfo(Index);
				}
				else bSelected = false;
			}
		}
		else //있는데
		{
			if(bSelected) bSelected = false;
			else 
			{
				//임시로 비슷하게..
				SetRenderOpacity(1);

				PerkWidget->bHCodeSelected = false;
				PerkWidget->Point += NeedPoint;
				PerkWidget->UpdateSelectInfo(Index);
			}
		}
	}
	else //그 외
	{
		if(bSelected)
		{
			if(PerkWidget->Point >= NeedPoint)
			{
				SetRenderOpacity(0.5);
			
				PerkWidget->Point -= NeedPoint;
				PerkWidget->UpdateSelectInfo(Index);
			}
			else bSelected = false;
		}
		else
		{
			SetRenderOpacity(1);
		
			PerkWidget->Point += NeedPoint;
			PerkWidget->UpdateSelectInfo(Index);
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