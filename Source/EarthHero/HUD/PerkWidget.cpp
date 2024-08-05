// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkWidget.h"

#include "PerkButtonWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"

UPerkWidget::UPerkWidget(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//친구 초대 row 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> PerkButtonAsset(TEXT("UserWidget'/Game/Blueprints/HUD/BPW_PerkButton.BPW_PerkButton_C'"));
	if (PerkButtonAsset.Succeeded())
	{
		PerkButtonClass = PerkButtonAsset.Class;
	}
}

void UPerkWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 생성 및 초기화
	CreateButtons();
}


void UPerkWidget::CreateButtons()
{
	if(!PerkButtonClass) return;
	
	int i, j;
	for(i = 0; i < NumOfLevels; i++)
	{
		UVerticalBox* NewVerticalBox = NewObject<UVerticalBox>(this);
		Perk_Hb->AddChild(NewVerticalBox);
		if(NewVerticalBox)
		{
			for(j = 0; j < NumOfPerkPerLevel; j++)
			{
				UPerkButtonWidget* NewButton = Cast<UPerkButtonWidget>(CreateWidget(this, PerkButtonClass));
				if(NewButton)
				{
					UE_LOG(LogTemp, Warning, TEXT("index = %d"), i * NumOfLevels + NumOfPerkPerLevel);
					NewButton->Index = i * NumOfLevels + NumOfPerkPerLevel;
					NewButton->AddOnClick();
					NewVerticalBox->AddChild(NewButton);
				}
			}
		}
	}
}