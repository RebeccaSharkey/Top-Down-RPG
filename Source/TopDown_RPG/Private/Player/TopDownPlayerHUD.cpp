// Copyright Spxcebxr Games


#include "Player/TopDownPlayerHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/TopDownUserWidget.h"
#include "UI/WidgetController/TopDownOverlayWidgetController.h"

void ATopDownPlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	AddTopDownPlayerOverlay();
}

void ATopDownPlayerHUD::InitTopDownPlayerOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	checkf(TopDownPlayerOverlayToSpawn, TEXT("TopDownPlayerHUD - InitTopDownPlayerOverlay: TopDownPlayerOverlayToSpawn unititialized"));
	checkf(TopDownOverlayWidgetControllerClass, TEXT("TopDownPlayerHUD - InitTopDownPlayerOverlay: TopDownOverlayWidgetControllerClass unititialized"));

	if(!TopDownPlayerOverlay)
	{		
		TopDownPlayerOverlay = CreateWidget<UTopDownUserWidget>(PC, TopDownPlayerOverlayToSpawn);
	}

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

	TopDownOverlayWidgetController = NewObject<UTopDownOverlayWidgetController>(this, TopDownOverlayWidgetControllerClass);
	TopDownOverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
	TopDownOverlayWidgetController->BindCallbacksToDependencies();

	TopDownPlayerOverlay->SetWidgetController(TopDownOverlayWidgetController);
	
	TopDownOverlayWidgetController->BroadcastInitialValues();
	
	TopDownPlayerOverlay->AddToViewport();
	AddTopDownPlayerOverlay();
}

void ATopDownPlayerHUD::AddTopDownPlayerOverlay() const
{
	if(TopDownPlayerOverlay)
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

UTopDownOverlayWidgetController* ATopDownPlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	checkf(TopDownOverlayWidgetController, TEXT("TopDownPlayerHUD - GetOverlayWidgetController: TopDownOverlayWidgetController not set."));

	return TopDownOverlayWidgetController;
}
