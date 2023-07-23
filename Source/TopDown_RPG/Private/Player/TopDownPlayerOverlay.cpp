// Copyright Spxcebxr Games


#include "Character/Player/TopDownPlayerOverlay.h"
#include "Character/Player/TopDownPlayerController.h"
#include "Character/Player/TopDownPlayer.h"
#include "Components/ProgressBar.h"

void UTopDownPlayerOverlay::OverlaySetup()
{	
	if(GetOwningPlayer())
	{
		TopDownPlayerController = Cast<ATopDownPlayerController>(GetOwningPlayer());
		if(!TopDownPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerOverlay - OverlaySetup(): Unable to set TopDownPlayerController"));
			return;
		}

		TopDownPlayer = TopDownPlayerController->GetTopDownPlayer();
		if(!TopDownPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerOverlay - OverlaySetup(): Unable to set TopDownPlayer"));
			return;
		}

		if(!EndTurnButton)
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerOverlay - OverlaySetup(): Unable to bind to EndTurnButton"));
			return;
		}		

		if(!SpeedProgressBar)
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerOverlay - OverlaySetup(): Unable to bind to SpeedProgressBar"));
			return;
		}
	}

	SpeedProgressBar->SetPercent(100.f);
}
