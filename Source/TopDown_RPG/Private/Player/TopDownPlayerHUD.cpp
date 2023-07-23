// Copyright Spxcebxr Games


#include "Player/TopDownPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/TopDownUserWidget.h"

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
		TopDownPlayerOverlay = CreateWidget<UTopDownUserWidget>(PlayerController, TopDownPlayerOverlayToSpawn);
		TopDownPlayerOverlay->AddToViewport();
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
