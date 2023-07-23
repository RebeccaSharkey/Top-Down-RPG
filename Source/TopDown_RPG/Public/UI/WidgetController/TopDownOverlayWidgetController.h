// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TopDownWidgetController.h"
#include "TopDownOverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitPointsChangedSignature, float, NewHitPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHitPointsChangedSignature, float, NewMaxHitPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedChangedSignature, float, NewSpeed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxSpeedChangedSignature, float, NewMaxSpeed);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TOPDOWN_RPG_API UTopDownOverlayWidgetController : public UTopDownWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHitPointsChangedSignature OnHitPointsChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHitPointsChangedSignature OnMaxHitPointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnSpeedChangedSignature OnSpeedChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxSpeedChangedSignature OnMaxSpeedChanged;
};
