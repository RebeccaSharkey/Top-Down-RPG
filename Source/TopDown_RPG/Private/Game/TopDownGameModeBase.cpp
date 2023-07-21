// Copyright Spxcebxr Games


#include "Game/TopDownGameModeBase.h"

#include "ToolBuilderUtil.h"
#include "Game/TopDownPlayerStart.h"
#include "Kismet/GameplayStatics.h"

void ATopDownGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentPlayerIndex = 0;
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

		if(TopDownPlayerStart->PlayerStartIndex == CurrentPlayerIndex)
		{
			CurrentPlayerIndex++;
			return PlayerStart;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Failed to set a player start")));	
	return Super::ChoosePlayerStart_Implementation(Player);
}
