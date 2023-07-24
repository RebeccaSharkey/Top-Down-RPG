// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TopDownWidgetController.h"
#include "TopDownOverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitPointsChangedSignature, float, NewHitPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHitPointsChangedSignature, float, NewMaxHitPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedChangedSignature, float, NewSpeed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxSpeedChangedSignature, float, NewMaxSpeed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTurnChangedSignature, bool, NewIsTurn);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TOPDOWN_RPG_API UTopDownOverlayWidgetController : public UTopDownWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHitPointsChangedSignature OnHitPointsChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHitPointsChangedSignature OnMaxHitPointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnSpeedChangedSignature OnSpeedChanged;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxSpeedChangedSignature OnMaxSpeedChanged;

	UPROPERTY(BlueprintAssignable, Category="Player|Attributes")
	FOnPlayerTurnChangedSignature OnPlayerTurnChanged;
	

protected:
	void HitPointsChanged(const FOnAttributeChangeData& Data) const;
	void MaxHitPointsChanged(const FOnAttributeChangeData& Data) const;
	void SpeedChanged(const FOnAttributeChangeData& Data) const;
	void MaxSpeedChanged(const FOnAttributeChangeData& Data) const;
	
	UFUNCTION(BlueprintCallable)
	void PlayerTurnChanged(bool bNewTurn);

public:
	UFUNCTION(BlueprintCallable)
	void PlayerRequestedTurnChange();
};
