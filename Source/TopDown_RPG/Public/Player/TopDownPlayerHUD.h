// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownPlayerHUD.generated.h"

class UTopDownPlayerOverlay;
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
	TSubclassOf<UUserWidget> TopDownPlayerOverlayToSpawn;	
	TObjectPtr<UTopDownPlayerOverlay> TopDownPlayerOverlay;

public:
	void AddTopDownPlayerOverlay();
	void RemoveTopDownPlayerOverlay() const;
	UTopDownPlayerOverlay* GetTopDownPlayerOverlay() const;

};
