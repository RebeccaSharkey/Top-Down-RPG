// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "Interaction/TopDownTargetInterface.h"
#include "GameFramework/Character.h"
#include "TopDownCharacterBase.generated.h"

UCLASS()
class TOPDOWN_RPG_API ATopDownCharacterBase : public ACharacter, public ITopDownTargetInterface
{
	GENERATED_BODY()

public:
	ATopDownCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

public:	
	virtual void HighlightActor(EInteractionType IteractionType) override;
	virtual void UnHighlightActor() override;
};
