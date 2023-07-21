// Copyright Spxcebxr Games


#include "Character/Player/TopDownPlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "Character/Player/TopDownPlayerOverlay.h"

void ATopDownPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	AddTopDownPlayerOverlay();
}

void ATopDownPlayerHUD::AddTopDownPlayerOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if(!PlayerController || !TopDownPlayerOverlayToSpawn)
	{
		return;
	}

	if(!TopDownPlayerOverlay)
	{		
		TopDownPlayerOverlay = CreateWidget<UTopDownPlayerOverlay>(PlayerController, TopDownPlayerOverlayToSpawn);
		TopDownPlayerOverlay->AddToViewport();
		TopDownPlayerOverlay->OverlaySetup();
	}
	else
	{
		TopDownPlayerOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATopDownPlayerHUD::RemoveTopDownPlayerOverlay() const
{
	if(TopDownPlayerOverlay)
	{
		TopDownPlayerOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

UTopDownPlayerOverlay* ATopDownPlayerHUD::GetTopDownPlayerOverlay() const
{
	return TopDownPlayerOverlay;
}
