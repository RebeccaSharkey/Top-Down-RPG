// Copyright Spxcebxr Games


#include "UI/WidgetController/TopDownOverlayWidgetController.h"

#include "AbilitySystem/TopDownAttributeSet.h"
#include "Player/TopDownPlayerState.h"

void UTopDownOverlayWidgetController::BroadcastInitialValues()
{
	//Super::BroadcastInitialValues();
	const ATopDownPlayerState* TopDownPlayerState = CastChecked<ATopDownPlayerState>(PlayerState);
	
	OnPlayerTurnChanged.Broadcast(TopDownPlayerState->GetPlayerTurn());

	const UTopDownAttributeSet* TopDownAttributeSet = CastChecked<UTopDownAttributeSet>(AttributeSet);
	
	OnHitPointsChanged.Broadcast(TopDownAttributeSet->GetHitPoints());
	OnMaxHitPointsChanged.Broadcast(TopDownAttributeSet->GetMaxHitPoints());
	
	OnSpeedChanged.Broadcast(TopDownAttributeSet->GetSpeed());
	OnMaxSpeedChanged.Broadcast(TopDownAttributeSet->GetMaxSpeed());
}

void UTopDownOverlayWidgetController::BindCallbacksToDependencies()
{
	//Super::BindCallbacksToDependencies();
	ATopDownPlayerState* TopDownPlayerState = CastChecked<ATopDownPlayerState>(PlayerState);

	TopDownPlayerState->OnPlayerTurnChanged.AddDynamic(this, &UTopDownOverlayWidgetController::PlayerTurnChanged);
	
	const UTopDownAttributeSet* TopDownAttributeSet = CastChecked<UTopDownAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownAttributeSet->GetHitPointsAttribute()).AddUObject(this, &UTopDownOverlayWidgetController::HitPointsChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownAttributeSet->GetMaxHitPointsAttribute()).AddUObject(this, &UTopDownOverlayWidgetController::MaxHitPointsChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownAttributeSet->GetSpeedAttribute()).AddUObject(this, &UTopDownOverlayWidgetController::SpeedChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownAttributeSet->GetMaxSpeedAttribute()).AddUObject(this, &UTopDownOverlayWidgetController::MaxSpeedChanged);
	
}

void UTopDownOverlayWidgetController::HitPointsChanged(const FOnAttributeChangeData& Data) const
{
	OnHitPointsChanged.Broadcast(Data.NewValue);
}

void UTopDownOverlayWidgetController::MaxHitPointsChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHitPointsChanged.Broadcast(Data.NewValue);
}

void UTopDownOverlayWidgetController::SpeedChanged(const FOnAttributeChangeData& Data) const
{
	OnSpeedChanged.Broadcast(Data.NewValue);
}

void UTopDownOverlayWidgetController::MaxSpeedChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxSpeedChanged.Broadcast(Data.NewValue);
}

void UTopDownOverlayWidgetController::PlayerTurnChanged(bool bNewTurn)
{
	OnPlayerTurnChanged.Broadcast(bNewTurn);
}

void UTopDownOverlayWidgetController::PlayerRequestedTurnChange()
{
	ATopDownPlayerState* TopDownPlayerState = CastChecked<ATopDownPlayerState>(PlayerState);
	if(TopDownPlayerState)
	{
		TopDownPlayerState->PlayerRequestedEndTurn();
	}
}

