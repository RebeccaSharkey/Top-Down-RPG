// Copyright Spxcebxr Games


#include "AbilitySystem/TopDownAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UTopDownAttributeSet::UTopDownAttributeSet()
{
	
}

void UTopDownAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTopDownAttributeSet, HitPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTopDownAttributeSet, MaxHitPoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTopDownAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTopDownAttributeSet, MaxSpeed, COND_None, REPNOTIFY_Always);
}

void UTopDownAttributeSet::OnRep_HitPoints(const FGameplayAttributeData& OldHitPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTopDownAttributeSet, HitPoints, OldHitPoints);
}

void UTopDownAttributeSet::OnRep_MaxHitPoints(const FGameplayAttributeData& OldMaxHitPoints) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTopDownAttributeSet, MaxHitPoints, OldMaxHitPoints);
}

void UTopDownAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTopDownAttributeSet, Speed, OldSpeed);
}

void UTopDownAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTopDownAttributeSet, MaxSpeed, OldMaxSpeed);
}
