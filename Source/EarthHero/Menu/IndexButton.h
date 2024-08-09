#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "IndexButton.generated.h"

class UPerkWidget;
class UDataTable;
struct FPerkStructure;

UCLASS()
class EARTHHERO_API UIndexButton : public UButton
{
	GENERATED_BODY()

	int NeedPoint = -1;
	bool bSelected = false;
	bool bHCode = false;
	
public:
	int Index;
	UPerkWidget* PerkWidget;
	UDataTable* PerkDataTable;
	
	void InitSetting(int ReceivedIndex, UPerkWidget* ParentWidget, UDataTable* DataTable);
	UFUNCTION()
	void IndexBtnClicked();
	UFUNCTION()
	void ButtonHovered();
	UFUNCTION()
	void ButtonUnhovered();
};
