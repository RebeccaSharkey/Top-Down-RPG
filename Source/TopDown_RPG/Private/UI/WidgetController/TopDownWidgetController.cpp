// Copyright Spxcebxr Games


#include "UI/WidgetController/TopDownWidgetController.h"

void UTopDownWidgetController::SetWidgetControllerParams(const FWidgetControllerParams WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTopDownWidgetController::BroadcastInitialValues()
{
	
}
