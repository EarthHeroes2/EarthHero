#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EarthHero/EHGameInstance.h"
#include "EarthHero/Stat/Structure/PerkStructure.h"
#include "Engine/DataTable.h" 
#include "PerkWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UIndexButton;
class UHorizontalBox;

UCLASS()
class EARTHHERO_API UPerkWidget : public UUserWidget
{
	GENERATED_BODY()

	UPerkWidget(const FObjectInitializer &ObjectInitializer);
	virtual bool Initialize() override;

public:
	int Point;
	int SelectedPerksCount = 0;
	const int MaxSelectedPerks = 6;
	int Level;
	
private:
	TArray<UIndexButton*> Buttons;

	void CreateButtons();

	UFUNCTION()
	void PerkSaveBtnClicked();
	UFUNCTION()
	void PerkCancelBtnClicked();

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Perk_Hb;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* EquippedPerks_Hb;

	UPROPERTY(meta = (BindWidget))
	UButton* PerkSave_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* PerkCancel_Btn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Level_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Point_Tb;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PerkName_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PerkCost_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PerkDescription_Tb;
	UPROPERTY(meta = (BindWidget))
	UImage* PerkImage_Img;

	int64 SelectInfo;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Perks")
	UDataTable* PerkDataTable;
	UEHGameInstance* EHGameInstance;
	void UpdateSelectInfo(int Index);
	void PerkButtonHovered(int Index);
	void PerkButtonUnhovered(int Index);
	void AddEquippedPerkImage(UTexture2D* PerkTexture);
	void RemoveEquippedPerkImage(UTexture2D* PerkTexture);

	bool bHCodeSelected = false;
};
