#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "IndexButton.generated.h"

class PerkInfomation;
class UTextBlock;
class UEHGameInstance;
class UPerkWidget;
/**
 * 
 */
UCLASS()
class EARTHHERO_API UIndexButton : public UButton
{
	GENERATED_BODY()

	UEHGameInstance* EHGameInstance;

	PerkInfomation* PerkInfo;
	
	int NeedPoint = -1;
	
	bool bSelected = false;
	bool bHCode = false;
	
public:
	int Index;
	UPerkWidget* PerkWidget;
	
	void InitSetting(int ReceivedIndex, UPerkWidget* ParentWidget);
	UFUNCTION()
	void IndexBtnClicked();
	UFUNCTION()
	void ButtonHovered();
	UFUNCTION()
	void ButtonUnhovered();
};
