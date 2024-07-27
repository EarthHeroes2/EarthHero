// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameViewportClient.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UCustomGameViewportClient::PostRender(UCanvas* Canvas)
{
	Super::PostRender(Canvas);
	
	if (bFading)
	{
		DrawScreenFade(Canvas);
	}
}

void UCustomGameViewportClient::ClearFade()
{
	bFading = false;
}

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
			//FPlatformTime::Seconds....?
			//fade start로 부터 FadeDuration 만큼 지나면 alpha는 1이 되고...
			const float Time = World_->GetTimeSeconds();
			const float Alpha = FMath::Clamp((Time - FadeStartTime) / FadeDuration, 0.f, 1.f);

			//새 맵으로 이동한 시점이라면 종료
			if(Time == 0.f)
			{
				//bFading = false;
				Fade(1.f, false);
			}
			

			UE_LOG(LogTemp, Warning, TEXT("Alpha = %f, bToBlack = %d, Time = %f, FadeStartTime = %f, FadeDuration = %f,    bFading = %d"), Alpha, bToBlack, Time, FadeStartTime, FadeDuration, bFading);
			
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