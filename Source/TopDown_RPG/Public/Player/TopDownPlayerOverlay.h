// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TopDownPlayerOverlay.generated.h"

class ATopDownPlayerController;
class ATopDownPlayer;
class UButton;
class UProgressBar;

/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API UTopDownPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void OverlaySetup();

private:
	TObjectPtr<ATopDownPlayerController> TopDownPlayerController;
	TObjectPtr<ATopDownPlayer> TopDownPlayer;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> EndTurnButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> SpeedProgressBar;
};
