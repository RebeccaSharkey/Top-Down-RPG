// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TopDownPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTurnChangedHandle, bool, bNewTurn);

class ATopDownCharacter;
class ATopDownPlayerController;
class ATopDownPlayer;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATopDownPlayerState();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetUpPlayerState(ATopDownPlayer* NewTopDownPlayer);
	
protected:
	UPROPERTY(VisibleAnywhere, Replicated, Category="PlayerVariables")
	TObjectPtr<ATopDownPlayer> TopDownPlayer;
	UPROPERTY(VisibleAnywhere, Replicated, Category="PlayerVariables")
	TObjectPtr<ATopDownPlayerController> TopDownPlayerController;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_CurrentTopDownCharacter, Category="PlayerVariables")
	TObjectPtr<ATopDownCharacter> CurrentTopDownCharacter;
	UPROPERTY(VisibleAnywhere, Replicated, Category="PlayerVariables")
	TObjectPtr<UAbilitySystemComponent> CurrentCharactersAbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, Replicated, Category="PlayerVariables")
	TObjectPtr<UAttributeSet> CurrentCharactersAttributeSet;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	void SetCurrentCharacter(ATopDownCharacter* CurrentCharacter);

private:
	UFUNCTION()
	void OnRep_CurrentTopDownCharacter();

	/* Players Turns Set Up */
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_bIsPlayersTurnChange, Category="PlayerVariables")
	bool bIsPlayersTurn;

public:
	void SetPlayerTurn(bool bInTurn);

private:
	UFUNCTION()
	void OnRep_bIsPlayersTurnChange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Pathing", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UGameplayEffect> StartTurnGameplayEffect;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerTurnChangedHandle OnPlayerTurnChanged;
	
	bool GetPlayerTurn() const;
	void PlayerRequestedEndTurn();
	
private:
	UFUNCTION(Server, Reliable)
	void Server_EndTurn();

	
};
