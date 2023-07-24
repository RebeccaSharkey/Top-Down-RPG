// Copyright Spxcebxr Games


#include "Game/TopDownGameModeBase.h"
#include "Game/TopDownPlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TopDownPlayerController.h"
#include "Player/TopDownPlayerState.h"

void ATopDownGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentPlayerStartIndex = 0;
	UGameplayStatics::GetAllActorsOfClass(this, ATopDownPlayerStart::StaticClass(), PlayerStarts);	
}

AActor* ATopDownGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if(PlayerStarts.Num() <= 0 )
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}
	
	for(AActor* PlayerStart : PlayerStarts)
	{
		ATopDownPlayerStart* TopDownPlayerStart = Cast<ATopDownPlayerStart>(PlayerStart);
		if(!TopDownPlayerStart)
		{
			continue;
		}

		if(TopDownPlayerStart->PlayerStartIndex == CurrentPlayerStartIndex)
		{
			CurrentPlayerStartIndex++;
			return PlayerStart;
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void ATopDownGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(ATopDownPlayerState* PlayerState = NewPlayer->GetPlayerState<ATopDownPlayerState>())
	{
		Cast<ATopDownPlayerController>(NewPlayer)->SetPlayerIndex(CurrentPlayerIndex);

		TopDownPlayerStates.Add(PlayerState);
		
		if(CurrentPlayerIndex == 0)
		{
			PlayerState->SetPlayerTurn(true);
		}
		else
		{
			PlayerState->SetPlayerTurn(false);
		}

		CurrentPlayerIndex++;
	}
}

void ATopDownGameModeBase::EndTurn()
{
	TopDownPlayerStates[PlayerStatesIndex]->SetPlayerTurn(false);
	
	if(PlayerStatesIndex >= TopDownPlayerStates.Num() - 1)
	{
		PlayerStatesIndex = 0;
	}
	else
	{
		PlayerStatesIndex++;
	}
	
	TopDownPlayerStates[PlayerStatesIndex]->SetPlayerTurn(true);
}
