// Copyright Spxcebxr Games

#pragma once

#include "CoreMinimal.h"
#include "NavigationData.h"
#include "Character/TopDownCharacterBase.h"
#include "TopDownCharacter.generated.h"

class ATopDownCharacterController;
class ATopDownPlayerController;
class ATopDownPlayerState;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FNavigationPath;

/**
 * 
 */
UCLASS()
class TOPDOWN_RPG_API ATopDownCharacter : public ATopDownCharacterBase
{
	GENERATED_BODY()

public:
	friend class ATopDownPlayer;
	
	ATopDownCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:	
	virtual void BeginPlay() override;
	virtual void SetUpTopDownPlayerCharacter();

public:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	bool bIsInitialized = false;
	
	TObjectPtr<ATopDownCharacterController> TopDownCharacterController;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Player", meta = (AllowPrivateAccess = "true"))	
	TObjectPtr<ATopDownPlayer> TopDownOwnerPlayer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category="Player", meta = (AllowPrivateAccess = "true"))	
	TObjectPtr<ATopDownPlayerState> TopDownOwnerPlayerState;
	
	/* Movement*/
	void CheckPath(FVector Location);	
	FNavPathSharedPtr PathSharedPtr;

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerPathingVariables(FVector Location);	
	void MoveTo(FVector Location);	
};
