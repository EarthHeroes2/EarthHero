// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SocketClient.generated.h"

enum ERequest : int;
/**
 * 
 */
UCLASS()
class EARTHHERO_API USocketClient : public UObject
{
	GENERATED_BODY()
	
public:
	FString CreateSocket(const ERequest RequestMessage, const FString& ExtraInfo);
	TArray<FString> StringTokenizer(FString Str);
	
};
