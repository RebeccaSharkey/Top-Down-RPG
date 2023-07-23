// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownPlayerHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UTopDownOverlayWidgetController;
class UTopDownUserWidget;
/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownPlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	/* Top Down Player Overlay */
private:
	UPROPERTY(EditDefaultsOnly, Category="PlayerOverlay", meta=(AllowPrivateAccess="True"))
	TSubclassOf<UTopDownUserWidget> TopDownPlayerOverlayToSpawn;	
	TObjectPtr<UTopDownUserWidget> TopDownPlayerOverlay;

public:
	void InitTopDownPlayerOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void AddTopDownPlayerOverlay() const;
	void RemoveTopDownPlayerOverlay() const;

	/* Top Down Player Overlay Widget Controller */
private:
	UPROPERTY(EditDefaultsOnly, Category="PlayerOverlay", meta=(AllowPrivateAccess="True"))
	TSubclassOf<UTopDownOverlayWidgetController> TopDownOverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UTopDownOverlayWidgetController> TopDownOverlayWidgetController;
	
public:
	UTopDownOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
};
