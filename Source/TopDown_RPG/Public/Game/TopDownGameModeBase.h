// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameModeBase.generated.h"

class ATopDownPlayerState;
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
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	TArray<TObjectPtr<AActor>> PlayerStarts;
	int32 CurrentPlayerStartIndex = 0;
	int32 CurrentPlayerIndex = 0;

	int32 PlayerStatesIndex = 0;
	TArray<ATopDownPlayerState*> TopDownPlayerStates;

public:
	void EndTurn();
};
