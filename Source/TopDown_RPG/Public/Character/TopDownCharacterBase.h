// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "Interaction/TopDownTargetInterface.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TopDownCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class TOPDOWN_RPG_API ATopDownCharacterBase : public ACharacter, public ITopDownTargetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATopDownCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/* IAbilitySystemInterface */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>	AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	/* ITopDownTargetInterface */
	virtual void HighlightActor(EInteractionType IteractionType) override;
	virtual void UnHighlightActor() override;
	
};
