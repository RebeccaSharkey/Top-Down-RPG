// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownPlayerHUD.generated.h"

class UTopDownUserWidget;
/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="PlayerOverlay", meta=(AllowPrivateAccess="True"))
	TSubclassOf<UTopDownUserWidget> TopDownPlayerOverlayToSpawn;	
	TObjectPtr<UTopDownUserWidget> TopDownPlayerOverlay;

public:
	void AddTopDownPlayerOverlay();
	void RemoveTopDownPlayerOverlay() const;
};
