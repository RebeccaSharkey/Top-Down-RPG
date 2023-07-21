// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TopDownPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category="CustomTopDownValues")
	int32 PlayerStartIndex;
};
