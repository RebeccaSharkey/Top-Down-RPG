// Copyright Spxcebxr Games


#include "UI/WidgetController/TopDownOverlayWidgetController.h"

#include "AbilitySystem/TopDownAttributeSet.h"

void UTopDownOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UTopDownAttributeSet* TopDownAttributeSet = CastChecked<UTopDownAttributeSet>(AttributeSet);
	
	OnHitPointsChanged.Broadcast(TopDownAttributeSet->GetHitPoints());
	OnMaxHitPointsChanged.Broadcast(TopDownAttributeSet->GetMaxHitPoints());
	OnSpeedChanged.Broadcast(TopDownAttributeSet->GetSpeed());
	OnMaxSpeedChanged.Broadcast(TopDownAttributeSet->GetMaxSpeed());
}
