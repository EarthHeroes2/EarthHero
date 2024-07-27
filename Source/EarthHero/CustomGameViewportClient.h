// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "CustomGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class EARTHHERO_API UCustomGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	virtual void PostRender(UCanvas* Canvas) override;
	
	virtual void ClearFade();
	
	virtual void Fade(const float Duration, const bool bToBlack);
	
	void DrawScreenFade(UCanvas* Canvas);

private:

	// Values used by our screen fading
	bool bFading = false;
	bool bToBlack = false;
	float FadeAlpha;
	float FadeStartTime;
	float FadeDuration;
};
