#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "LobbyRowWidget.generated.h"


class UMainMenuWidget;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class EARTHHERO_API ULobbyRowWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize() override;
	
	FOnlineSessionSearchResult LobbyInfo;
	bool bAdvertise;

	UPROPERTY(meta = (BindWidget))
	UButton* LobbyJoin_Btn;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LobbyName_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCount_Tb;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Ping_Tb;
	
	UFUNCTION()
	void JoinClicked();

public:
	UMainMenuWidget* MainMenuWidget;
	
	void UpdateLobbyInfo(FOnlineSessionSearchResult Lobby);
};
