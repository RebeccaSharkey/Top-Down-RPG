// Copyright Spxcebxr Games


#include "Character/Enemy/TopDownEnemy.h"
#include "AbilitySystem/TopDownAbilitySystemComponent.h"
#include "AbilitySystem/TopDownAttributeSet.h"

ATopDownEnemy::ATopDownEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTopDownAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTopDownAttributeSet>("AttributeSet");
}
