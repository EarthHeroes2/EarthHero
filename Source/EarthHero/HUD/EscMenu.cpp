#include "EscMenu.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UEscMenu::UEscMenu(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//옵션 블루프린트
	static ConstructorHelpers::FClassFinder<UUserWidget> OptionsWidgetAsset(TEXT("UserWidget'/Game/Blueprints/Menu/WBP_Options.WBP_Options_C'"));
	if (OptionsWidgetAsset.Succeeded())
	{
		OptionsWidgetClass = OptionsWidgetAsset.Class;
	}
}

void UEscMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeBtn)
	{
		ResumeBtn->OnClicked.AddDynamic(this, &UEscMenu::OnResumeClicked);
	}

	if (SettingsBtn)
	{
		SettingsBtn->OnClicked.AddDynamic(this, &UEscMenu::OnSettingsClicked);
	}

	if (LeaveBtn)
	{
		LeaveBtn->OnClicked.AddDynamic(this, &UEscMenu::OnLeaveClicked);
	}
}

void UEscMenu::OnResumeClicked()
{
	SetVisibility(ESlateVisibility::Hidden);

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// Game Only 모드
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UEscMenu::OnSettingsClicked()
{
	if (OptionsWidget)
	{
		if (OptionsWidget->IsVisible())
			OptionsWidget->SetVisibility(ESlateVisibility::Hidden);
		else
			OptionsWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OptionsWidget = Cast<UUserWidget>(CreateWidget(this, OptionsWidgetClass));
		if (OptionsWidget)
		{
			OptionsWidget->AddToViewport();
			OptionsWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UEscMenu::OnLeaveClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		PlayerController->ClientTravel("/Game/Maps/StartupMap", ETravelType::TRAVEL_Absolute);
	}
}
