// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TopDownAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API UTopDownAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTopDownAttributeSet();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HitPoints, Category="Primary Attributes")
	FGameplayAttributeData HitPoints;
	UFUNCTION()
	void OnRep_HitPoints(const FGameplayAttributeData& OldHitPoints) const;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHitPoints, Category="Primary Attributes")
	FGameplayAttributeData MaxHitPoints;
	UFUNCTION()
	void OnRep_MaxHitPoints(const FGameplayAttributeData& OldMaxHitPoints) const;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Speed, Category="Primary Attributes")
	FGameplayAttributeData Speed;
	UFUNCTION()
	void OnRep_Speed(const FGameplayAttributeData& OldSpeed) const;	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxSpeed, Category="Primary Attributes")
	FGameplayAttributeData MaxSpeed;
	UFUNCTION()
	void OnRep_MaxSpeed(const FGameplayAttributeData& OldMaxSpeed) const;
	
};
