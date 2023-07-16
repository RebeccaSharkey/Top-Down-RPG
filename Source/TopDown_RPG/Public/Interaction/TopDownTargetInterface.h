// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TopDownTargetInterface.generated.h"

#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_GREEN 251
#define CUSTOM_DEPTH_WHITE 252

UENUM(BlueprintType)
enum EInteractionType : uint8
{
	EIT_DEFAULT UMETA(DisplayName="Default"),
	EIT_Player	UMETA(DisplayName="Player"),
	EIT_Enemy	UMETA(DisplayName="Enemy"),
	EIT_Item	UMETA(DisplayName="Item"),
	EIT_MAX	UMETA(DisplayName="MAX")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTopDownTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOPDOWN_RPG_API ITopDownTargetInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightActor(EInteractionType IteractionType) = 0;
	virtual void UnHighlightActor() = 0;
};
