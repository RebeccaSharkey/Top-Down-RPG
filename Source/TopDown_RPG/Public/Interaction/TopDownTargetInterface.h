// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TopDownTargetInterface.generated.h"

#define CUSTOM_DEPTH_RED 250

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
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
