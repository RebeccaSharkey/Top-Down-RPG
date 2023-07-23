// Copyright Spxcebxr Games


#include "Player/TopDownPlayerState.h"
#include "Player/TopDownPlayerController.h"
#include "Character/PlayerCharacter/TopDownCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Player/TopDownPlayer.h"
#include "Player/TopDownPlayerHUD.h"

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

void ATopDownPlayerState::SetUpPlayerState(ATopDownPlayer* NewTopDownPlayer)
{
	if(GetPlayerController())
	{
		TopDownPlayerController = Cast<ATopDownPlayerController>(GetPlayerController());
		if(!TopDownPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("TopDownPlayerState - SetUpPlayerState: Failed to set TopDownPlayerController"));
			return;
		}

		TopDownPlayer = NewTopDownPlayer;
		if(!TopDownPlayer)
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
	if(CurrentCharacter)
	{
		CurrentCharactersAbilitySystemComponent = CurrentCharacter->GetAbilitySystemComponent();
		CurrentCharactersAttributeSet = CurrentCharacter->GetAttributeSet();		
	}
	else
	{
		CurrentCharactersAbilitySystemComponent = nullptr;
		CurrentCharactersAttributeSet = nullptr;
	}
	
	CurrentTopDownCharacter = CurrentCharacter;
}

void ATopDownPlayerState::OnRep_CurrentTopDownCharacter()
{
	if(!TopDownPlayer || !TopDownPlayerController || !CurrentTopDownCharacter || !CurrentCharactersAbilitySystemComponent || !CurrentCharactersAttributeSet)
	{
		return;
	}

	if(!TopDownPlayer->IsLocallyControlled())
	{
		 return;
	}

	ATopDownPlayerHUD* TopDownPlayerHUD = Cast<ATopDownPlayerHUD>(TopDownPlayerController->GetHUD());
	if(!TopDownPlayerHUD)
	{
		return;
	}

	TopDownPlayerHUD->InitTopDownPlayerOverlay(TopDownPlayerController, this, CurrentCharactersAbilitySystemComponent, CurrentCharactersAttributeSet);
}
