// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TopDownWidgetController.generated.h"

class ATopDownPlayerController;
class ATopDownPlayerState;
class UAbilitySystemComponent;
class UAttributeSet;


/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API UTopDownWidgetController : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<ATopDownPlayerController> TopDownPlayerController;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<ATopDownPlayerState> TopDownPlayerState;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
