// Copyright Spxcebxr Games


#include "Player/TopDownPlayerState.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/TopDownAbilitySystemComponent.h"
#include "Player/TopDownPlayerController.h"
#include "Character/PlayerCharacter/TopDownCharacter.h"
#include "Game/TopDownGameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/TopDownPlayer.h"
#include "Player/TopDownPlayerHUD.h"
#include "AbilitySystemBlueprintLibrary.h"

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
	DOREPLIFETIME(ATopDownPlayerState, bIsPlayersTurn);
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

void ATopDownPlayerState::OnRep_bIsPlayersTurnChange()
{
	if(!TopDownPlayer)
	{
		return;
	}
	
	if(TopDownPlayer->IsLocallyControlled())
	{
		OnPlayerTurnChanged.Broadcast(bIsPlayersTurn);
	}
}

void ATopDownPlayerState::SetPlayerTurn(bool bInTurn)
{
	bIsPlayersTurn = bInTurn;

	if(!bIsPlayersTurn)
	{
		return;
	}

	UTopDownAbilitySystemComponent* TopDownAbilitySystemComponent = Cast<UTopDownAbilitySystemComponent>(CurrentCharactersAbilitySystemComponent);

	if(!TopDownAbilitySystemComponent)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = TopDownAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec = TopDownAbilitySystemComponent->MakeOutgoingSpec(StartTurnGameplayEffect, 1.f, EffectContext);

	TopDownAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

bool ATopDownPlayerState::GetPlayerTurn() const
{
	return bIsPlayersTurn;
}

void ATopDownPlayerState::PlayerRequestedEndTurn()
{
	if(bIsPlayersTurn)
	{		
		Server_EndTurn();
	}
}

void ATopDownPlayerState::Server_EndTurn_Implementation()
{
	ATopDownGameModeBase* TopDownGameModeBase = Cast<ATopDownGameModeBase>(GetWorld()->GetAuthGameMode());

	if(!TopDownGameModeBase)
	{
		return;
	}
	
	TopDownGameModeBase->EndTurn();
}
