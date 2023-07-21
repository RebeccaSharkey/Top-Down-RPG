// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameModeBase.generated.h"

class APlayerStart;
/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	TArray<AActor*> PlayerStarts;

	int32 CurrentPlayerIndex = 0;
	
};
