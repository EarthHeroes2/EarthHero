// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameViewportClient.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

void UCustomGameViewportClient::PostRender(UCanvas* Canvas)
{
	Super::PostRender(Canvas);
	
	if (bFading) DrawScreenFade(Canvas);
}

void UCustomGameViewportClient::ClearFade()
{
	bFading = false;
}

//수동으로 실행시켜 주면 됨
void UCustomGameViewportClient::Fade(const float Duration, const bool bToBlack_)
{
	const UWorld* World_ = GetWorld();
	if (World_)
	{
		bFading = true;
		FadeDuration = Duration;
		bToBlack = bToBlack_;
		FadeStartTime = World_->GetTimeSeconds();
	}
}

void UCustomGameViewportClient::DrawScreenFade(UCanvas* Canvas)
{
	if (bFading)
	{
		const UWorld* World_ = GetWorld();
		if (World_)
		{
			const float Time = World_->GetTimeSeconds();
			const float Alpha = FMath::Clamp((Time - FadeStartTime) / FadeDuration, 0.f, 1.f);
			
			//새 맵으로 이동한 시점(seamless travel 아닐 때!!!!)이라면 자동으로 페이드 인
			//if(Time == 0.f) Fade(1.5f, false);
			
			if (Alpha == 1.f && !bToBlack) bFading = false;
			else
			{
				FColor OldColor = Canvas->DrawColor;
				FLinearColor FadeColor = FLinearColor::Black;
				FadeColor.A = bToBlack ? Alpha : 1 - Alpha;
				Canvas->DrawColor = FadeColor.ToFColor(true);
				Canvas->DrawTile(Canvas->DefaultTexture, 0, 0, Canvas->ClipX, Canvas->ClipY, 0, 0, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY());
				Canvas->DrawColor = OldColor;
			}
		}
	}
}