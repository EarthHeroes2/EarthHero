// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SocketClient.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API USocketClient : public UObject
{
	GENERATED_BODY()
	
public:
	FString GetLobbyNameAndPrivateState();
	FString GetPortNumber();
	FString CreateSocket(FString RequestMessage);
	
};
