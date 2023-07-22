// Copyright Spxcebxr Games


#include "Character/Player/TopDownPlayerState.h"
#include "AbilitySystem/TopDownAbilitySystemComponent.h"
#include "AbilitySystem/TopDownAttributeSet.h"
#include "Character/Player/TopDownPlayerController.h"
#include "Character/PlayerCharacter/TopDownCharacter.h"
#include "Net/UnrealNetwork.h"

ATopDownPlayerState::ATopDownPlayerState()
{
	NetUpdateFrequency = 100.f;
	bReplicates = true;
}

void ATopDownPlayerState::BeginPlay()
{
	Super::BeginPlay();		
}

void ATopDownPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);	

	DOREPLIFETIME(ATopDownPlayerState, TopDownPlayer);
	DOREPLIFETIME_CONDITION(ATopDownPlayerState, TopDownPlayerController, COND_OwnerOnly);
	DOREPLIFETIME(ATopDownPlayerState, CurrentTopDownCharacter);
	DOREPLIFETIME(ATopDownPlayerState, CurrentCharactersAbilitySystemComponent);
	DOREPLIFETIME(ATopDownPlayerState, CurrentCharactersAttributeSet);
}

void ATopDownPlayerState::SetUpPlayerState()
{
	if(GetPlayerController())
	{
		TopDownPlayerController = Cast<ATopDownPlayerController>(GetPlayerController());
		if(!TopDownPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerState - SetUpPlayerState: Failed to set TopDownPlayerController"));
			return;
		}

		if(TopDownPlayerController->GetTopDownPlayer())
		{
			TopDownPlayer = TopDownPlayerController->GetTopDownPlayer();
		}
		else
		{			
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerState - SetUpPlayerState: Failed to set TopDownPlayer"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerState - SetUpPlayerState: No Player Controller"));
	}
}

UAbilitySystemComponent* ATopDownPlayerState::GetAbilitySystemComponent() const
{
	return CurrentCharactersAbilitySystemComponent;
}

UAttributeSet* ATopDownPlayerState::GetAttributeSet() const
{
	return CurrentCharactersAttributeSet;
}

void ATopDownPlayerState::SetCurrentCharacter(ATopDownCharacter* CurrentCharacter)
{
	CurrentTopDownCharacter = CurrentCharacter;

	if(CurrentTopDownCharacter)
	{
		CurrentCharactersAbilitySystemComponent = CurrentCharacter->GetAbilitySystemComponent();
		CurrentCharactersAttributeSet = CurrentCharacter->GetAttributeSet();		
	}
	else
	{
		CurrentCharactersAbilitySystemComponent = nullptr;
		CurrentCharactersAttributeSet = nullptr;
	}
}
